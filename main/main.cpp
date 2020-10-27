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

void* HandleTCPClient(void* arg) {
    char buff_out[BUFFER_SZ];
    char message[BUFFER_SZ];
    int leave_flag = 0;
	Client* client = (Client*)arg;

	while(true) {
		if (leave_flag)
			break;
					
		int receive = recv(client->sockfd, buff_out, BUFFER_SZ, 0);
		if (receive > 0) {
            SendDataToClient(buff_out, client->sockfd);
		} else if (receive == 0 || strcmp(buff_out, "exit") == 0) {
			sprintf(buff_out, "%s has left", client->name.c_str());
			sysout(buff_out);
			leave_flag = 1;
		} else {
			printf("ERROR: -1\n");
			leave_flag = 1;
		}
		bzero(buff_out, BUFFER_SZ);
		bzero(message, BUFFER_SZ);
	}

	return NULL;
}
