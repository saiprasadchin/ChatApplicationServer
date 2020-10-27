#include "TcpListener.h"

TcpListener::TcpListener(const char* ipAddress, int port, HandleTCPClientPtr funPtr)
	: m_ipAddress(ipAddress), m_port(port), m_handle_client_ptr(funPtr) {}

void TcpListener::Run()
{
	char buffer[1024];
    SOCKET listening = CreateSocket();
    pthread_t tid;
	while (true) {
		Client* client = WaitForConnection(listening);
	    pthread_create(&tid, NULL, *m_handle_client_ptr, (void*)client);		
	}
    close(listening);
}

SOCKET TcpListener::CreateSocket()
{
    SOCKET listening;
    int option;
    sockaddr_in serv_addr;
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(m_ipAddress);
    serv_addr.sin_port = htons(m_port);

    ErrorCheck((listening = socket(AF_INET, SOCK_STREAM, 0)),
                "ERROR: Failed to create socket");

    ErrorCheck(setsockopt(listening, SOL_SOCKET,(SO_REUSEPORT | SO_REUSEADDR),(char*)&option,sizeof(option)),
                "ERROR: setsockopt failed");

    ErrorCheck(bind(listening, (struct sockaddr*)&serv_addr, sizeof(serv_addr)),
                "ERROR: Socket binding failed");

    ErrorCheck(listen(listening, 10),
                "ERROR: Socket listening failed");

	printf("=== WELCOME TO THE CHATROOM ===\n");

	return listening;
}

Client* TcpListener::WaitForConnection(SOCKET listenfd)
{
    struct sockaddr_in client_addr;
	socklen_t clilen = sizeof(client_addr);
	SOCKET connfd;
    ErrorCheck(connfd = accept(listenfd, (struct sockaddr*)&client_addr, &clilen),
                "ERROR: accept failed");

    Client* client = new Client();
	client->address = client_addr;
	client->sockfd = connfd;
	client->uid = uid++;

	return client;
}