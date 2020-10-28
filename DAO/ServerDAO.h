#pragma once
#include <bsoncxx/builder/stream/document.hpp>
#include <bsoncxx/json.hpp>
#include <mongocxx/client.hpp>
#include <mongocxx/stdx.hpp>
#include <mongocxx/uri.hpp>
#include <mongocxx/instance.hpp>

#include <iostream>
#include <string>

using bsoncxx::builder::stream::close_array;
using bsoncxx::builder::stream::close_document;
using bsoncxx::builder::stream::document;
using bsoncxx::builder::stream::finalize;
using bsoncxx::builder::stream::open_array;
using bsoncxx::builder::stream::open_document;
using bsoncxx::builder::basic::kvp;
using bsoncxx::builder::basic::make_array;
using bsoncxx::builder::basic::make_document;

using namespace std;

class ServerDAO
{
    mongocxx::instance inst{};
    mongocxx::client conn{mongocxx::uri{}};
    bsoncxx::builder::stream::document document{};
    vector<string> GetCollections(string);
    string GetSenderName(string, string);
    string GetCollectionName(string, string);
public:
    bool IsUserNameAlreadyExits(string);
    void AddClient(string, string);
    bool CheckForValidUserNameAndPassword(string, string);
    vector<string> GetClientsIfPendingMessages(string);
    vector<string> GetUnseenMessages(string, string);
    vector<string> GetAllMessages(string, string);
};
