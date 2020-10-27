#include <iostream>
#include "../Server/TcpListener.h"
#include "../DAO/ServerDAO.h"

#define BUFFER_SZ 2048
#define REGISTER "REGISTER"
#define INVALID "1"
#define VALID "0"
#define sysout(x) std::cout << x << std::endl
void* HandleTCPClient(void* arg);
ServerDAO database;

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

	while(1) {
		if(leave_flag)
			break;

		if(recv(client->sockfd, buff_out, 32, 0) <= 0 ) {
			printf("Didn't enter the name.\n");
			leave_flag = 1;
		} else {
			std::cout << "buffer"<< buff_out << std::endl;
		}

		vector<string> client_data = Split(buff_out,' ');
		if( 0 < client_data.size()) {
			if(client_data[0] == REGISTER ) {
				if(!database.IsUserNameAlreadyExits(client_data[1])) {
					database.AddClient(client_data[1], client_data[2]);
					SendDataToClient(INVALID, client->sockfd);			
				} else {
					SendDataToClient(VALID, client->sockfd);
				}
			}
		}
	}	

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
