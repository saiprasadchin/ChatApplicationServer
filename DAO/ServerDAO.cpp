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

vector<string> ServerDAO::GetCollections(string name) {
    vector<string> list;
    vector<string> listcoll = conn["ChatApplicationDB"].list_collection_names();
    for( int i = 0; i < listcoll.size(); ++i ) {
        if(strstr(listcoll[i].c_str(), name.c_str()) != 0) {
            list.push_back(listcoll[i]);
        }
        }
    return list;
}

string ServerDAO::GetSenderName(string collection_name, string receiver_name) {
    auto collection = conn["ChatApplicationDB"][collection_name];
    auto cursor = collection.find({});
    bsoncxx::document::element seenStatus, sender_name;
    for (auto &&data : cursor) {
        seenStatus = data["seen_status"];
        sender_name = data["sender"];
        if(string(seenStatus.get_utf8().value) == "unseen" && string(sender_name.get_utf8().value) != receiver_name) {
            return string(sender_name.get_utf8().value);
        }
    }
    return "";
}

vector<string> ServerDAO::GetClientsIfPendingMessages(string receiver_name) {
    vector<string> sender_list;
    vector<string> cllections_list = GetCollections(receiver_name);
    for( int i = 0; i < cllections_list.size(); ++i ) {
        string sender_name;
        if((sender_name = GetSenderName(cllections_list[i], receiver_name)) != "") {
           sender_list.push_back(sender_name);
        }
    }
    return sender_list;
}