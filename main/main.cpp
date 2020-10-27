#include <iostream>
#include "../Server/TcpListener.h"

#define BUFFER_SZ 2048
#define sysout(x) std::cout << x << std::endl
void* HandleTCPClient(void* arg);

int main() {
    TcpListener server("127.0.0.1", 54000, HandleTCPClient);
	server.Run();

    return 0;
}