#include "TcpListener.h"

TcpListener::TcpListener(const char* ipAddress, int port, HandleTCPClientPtr funPtr)
	: m_ipAddress(ipAddress), m_port(port), m_handle_client_ptr(funPtr) {}

void TcpListener::Run()
{
	char buffer[1024];
    SOCKET listening = CreateSocket();
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

