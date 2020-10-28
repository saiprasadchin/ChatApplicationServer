#include "ServerDAO.h"

bool ServerDAO::IsUserNameAlreadyExits(string userId) {
    auto collection = conn["ChatApplicationDB"]["user"];
    auto cursor = collection.find({});
    bsoncxx::document::element id;
    for (auto&& data : cursor) {
        id = data["userId"];
        if (string(id.get_utf8().value) == userId) {
           return true;
        }
    }
    return false;
}

void ServerDAO::AddClient(string userId, string password) {
    auto collection = conn["ChatApplicationDB"]["user"];
    auto builder = bsoncxx::builder::stream::document{};
    bsoncxx::document::value document = builder
                                        << "userId" << userId
                                        << "password" << password
                                        << finalize;

        collection.insert_one(document.view());
}

bool ServerDAO::CheckForValidUserNameAndPassword(string userId, string pass_word) {
    auto collection = conn["ChatApplicationDB"]["user"];
    auto cursor = collection.find({});
    bsoncxx::document::element id, password;

    for (auto data : cursor) {
        id = data["userId"];
        password = data["password"];
        if ((string(id.get_utf8().value) == userId) && (string(password.get_utf8().value) == pass_word)) {
            cout << "login sucessfull\n";
            return true;
        }
    }   
    return false;
}