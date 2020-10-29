#include "ServerDAO.h"
std::mutex mutex_lock;

bool ServerDAO::IsUserNameAlreadyExits(string userId) {
    std::lock_guard<std::mutex> lck (mutex_lock);
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
    std::lock_guard<std::mutex> lck (mutex_lock);
    auto collection = conn["ChatApplicationDB"]["user"];
    auto builder = bsoncxx::builder::stream::document{};
    bsoncxx::document::value document = builder
                                        << "userId" << userId
                                        << "password" << password
                                        << finalize;

        collection.insert_one(document.view());
}

bool ServerDAO::CheckForValidUserNameAndPassword(string userId, string pass_word) {
    std::lock_guard<std::mutex> lck (mutex_lock);
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
    std::lock_guard<std::mutex> lck (mutex_lock);
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
    std::lock_guard<std::mutex> lck (mutex_lock);
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
    std::lock_guard<std::mutex> lck (mutex_lock);
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

string ServerDAO::GetCollectionName(string sender, string receiver) {
    std::lock_guard<std::mutex> lck (mutex_lock);
    if (sender < receiver)
        return sender + "_" + receiver;
    return receiver + "_" + sender;
}

vector<string> ServerDAO::GetUnseenMessages(string sender_name, string receiver_name) {
    std::lock_guard<std::mutex> lck (mutex_lock);
    bsoncxx::document::element sender, receiver, message, seen_status;
    string collection_name = GetCollectionName(sender_name, receiver_name);
    auto collection = conn["ChatApplicationDB"][collection_name];
    vector<string> messagesList;
    auto cursor = collection.find({});
        for ( auto&& data : cursor) {
            sender = data["sender"];
            receiver = data["receiver"];
            message = data["message"];
            seen_status = data["seen_status"];
            if (string(receiver.get_utf8().value) == receiver_name && string(seen_status.get_utf8().value) == "unseen") {
                messagesList.push_back(string(message.get_utf8().value));
                collection.update_one(
                                    make_document(kvp("sender", sender_name),kvp("receiver", receiver_name),
                                    kvp("seen_status","unseen")),
                                    make_document( kvp("$set", make_document(kvp("seen_status", "seen")))));
            }
        }
    
    return messagesList;
}

void ServerDAO::AddMessageToDB(string sender, string receiver, string message, string seen_status) {
    std::lock_guard<std::mutex> lck (mutex_lock);
    string collectionName = GetCollectionName(sender, receiver);
    auto collection = conn["ChatApplicationDB"][collectionName];
        
    
    auto builder = bsoncxx::builder::stream::document{};
    bsoncxx::document::value document = builder
            << "sender" << sender
            << "receiver" << receiver
            << "message" << message
            << "seen_status" << seen_status
            << "time_stamp" << chrono::system_clock::to_time_t(chrono::system_clock::now())
            << finalize;

    collection.insert_one(document.view());
}

vector<string> ServerDAO::GetAllMessages(string sender_name, string receiver_name) {
    std::lock_guard<std::mutex> lck (mutex_lock);
    string collectionsName = GetCollectionName(sender_name, receiver_name);
    auto collection = conn["ChatApplicationDB"][collectionsName];
    auto cursor = collection.find({});
    bsoncxx::document::element sender, reciever, message;
    vector<string> messagesList;

    for (auto && data : cursor) {
        sender = data["sender"];
        message = data["message"];
        string all_messages;
        if (string(sender.get_utf8().value) == sender_name) {
            all_messages = "\x1B[36m" + string(sender.get_utf8().value) + "\033[0m" + ":" + string(message.get_utf8().value);
        } else {
            all_messages = "\033[1;32m" + string("YOU") + "\033[0m" + ":" + string(message.get_utf8().value);
        }
        messagesList.push_back(all_messages);
    }

    return messagesList;
}