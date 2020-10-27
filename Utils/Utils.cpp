#include "Utils.h"

int ErrorCheck(int expression, const char* message) {
    if(expression == SCOKETERROR) {
        perror(message);
        return EXIT_FAILURE;
    }
    return expression;
}

void SendDataToClient(std::string message, SOCKET sockfd) {
	if(write(sockfd, message.c_str(), message.size()) < 0) {
		perror("ERROR: write to descriptor failed");
	}
}

vector<string> Split(const string &s, char delim) {
    stringstream ss(s);
    string item;
    vector<string> tokens;
    while (getline(ss, item, delim)) {
        tokens.push_back(item);
    }
    return tokens;
}
