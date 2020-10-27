#pragma once
#include <netinet/in.h>
#include <string>

using namespace std;

class Client
{
public:
	struct sockaddr_in address;
	int sockfd;
	int uid;
	string name;
	string pass_word;
};
