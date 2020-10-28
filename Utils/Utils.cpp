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

string ConvertListToString(vector<string> clients_list, string flag) {
    string clients = flag;
    string temp;
    for(int i=0; i < clients_list.size(); ++i) {
        temp = clients_list[i];
        clients += temp;
        clients += " ";
    }
    return clients;
}

void OnLineClients(string name , SOCKET socket) {
	vector<string> clients = GetOnLineClients(name);
	string online_clients = ConvertListToString(clients, "ONLINE ");
	SendDataToClient(online_clients, socket);
}

void StringConcate(string& str, ...) {
    va_list pArg;
    va_start(pArg, str);
    const char *strNext = va_arg(pArg, const char *);
    while (strNext != NULL) {
        str += strNext;
        strNext = va_arg(pArg, const char *);
    }
    va_end(pArg);
}

void* parse(char* buffer, char* message, char* name) {

	int index = 0;
	int i;
	if (buffer[0] != '@' ) {
			name[index] = ' ';
      		return NULL;
    }
	
	for ( i = 1; i < strlen(buffer); i++) {
		if(buffer[i] != ' ') {
			name[index++] = buffer[i];
		} else {
			name[index++] = '\0';
			break;
		}
  	}
	index = 0;
	i++;
	for ( ; i < strlen(buffer); i++) {
		if(buffer[i] != '\0') {
			message[index++] = buffer[i];
		}
  	}	
	return NULL;
}
