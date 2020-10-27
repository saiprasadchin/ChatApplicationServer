#pragma once
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <pthread.h>
#include <sys/types.h>
#include <signal.h>
#include <iostream>
#include <string>
#include <vector>
#include "../Models/Client.h"
#include "../Utils/Utils.h"
#include <list>
#include <thread>

#define SOCKET int
#define ALREADY_EXIST "1"
using namespace std;

static int uid = 10;

typedef void* (*HandleTCPClientPtr)(void*);

class TcpListener 
{
public:
	TcpListener(const char* ipAddress, int port, HandleTCPClientPtr ptr);
	void Run();
private:
	SOCKET CreateSocket();
	Client* WaitForConnection(SOCKET);

	const char* m_ipAddress;
	int m_port;
    HandleTCPClientPtr m_handle_client_ptr;
};
