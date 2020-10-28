#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include "../Server/TcpListener.h"

#define SOCKET int
#define SCOKETERROR -1

int ErrorCheck(int expression, const char* message);
void SendDataToClient(std::string, SOCKET);
vector<string> Split(const string &s, char delim);
string ConvertListToString(vector<string> clients_list, string flag);