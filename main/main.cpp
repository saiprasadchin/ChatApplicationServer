#include "../DAO/OnLineDAO/OnLineDAO.h"
#include "../Server/TcpListener.h"
#include "../DAO/ServerDAO/ServerDAO.h"

#define BUFFER_SZ 2048
#define REGISTER "REGISTER"
#define AUTHENTICATE "AUTHENTICATE"
#define UNSEEN_MESSAGES_FROM "UNSEEN_MESSAGES_FROM"
#define MESSAGES "MESSAGES"
#define ALL_MESSAGES "ALL_MESSAGES"
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
	char name[32];
	char pass_word[10];
	string buffer_to_all = "";
	string buffer_to_particular = "";
	string main_buffer = "";
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

			if(client_data[0] == AUTHENTICATE ) {
				if(database.CheckForValidUserNameAndPassword(client_data[1], client_data[2])) {
					SendDataToClient(VALID, client->sockfd);
					client->name = client_data[1];
					client->pass_word = client_data[2];
					client_data[0] = "chat";
				} else {
					SendDataToClient(INVALID, client->sockfd);
				}
			}

			if(client_data[0] == UNSEEN_MESSAGES_FROM ) {
				vector<string> sender_list = database.GetClientsIfPendingMessages( client->name );
				string sender_clients_name = ConvertListToString( sender_list, "UNSEEN_MESSAGES_FROM ");
				SendDataToClient(sender_clients_name, client->sockfd);
			}

			if(client_data[0] == MESSAGES ) {
				vector<string> list = database.GetUnseenMessages(client_data[1], client->name );
				string all_messages = ConvertListToString( list, "UNSEEN_MESSAGES ");
				SendDataToClient(all_messages, client->sockfd);
			}

			if(client_data[0] == ALL_MESSAGES ) {
				vector<string> list = database.GetAllMessages(client_data[1], client->name);
				string all_messages = ConvertListToString( list, "ALL_MESSAGES ");
				SendDataToClient(all_messages, client->sockfd);

			}

			if(client_data[0] == "chat" ) {
				AddToQueue(client);
				bzero(buff_out, BUFFER_SZ);
				while(1) {
					if (leave_flag) {
						break;
					}
					int receive = recv(client->sockfd, buff_out, BUFFER_SZ, 0);
					if (receive > 0) {
						if(strcmp(buff_out, "ONLINE") == 0 ) {
							OnLineClients(client->name, client->sockfd);
						} else if(strcmp(buff_out, "--help") == 0 ) {
							SendDataToClient("$", client->sockfd);
							RemoveQueue(client->name);
							break;
						} else {
							StringConcate(buffer_to_all, client->name.c_str(), " : ", buff_out, NULL);
							if(strlen(buff_out) > 0) {
								parse(buff_out, message,name);
								StringConcate(buffer_to_particular,"\x1B[36m", client->name.c_str(),"\033[0m", " : ", message, NULL);
								if(name[0] == ' ') { 
									SendMessage(buffer_to_all.c_str(), client->uid);
								} else {
									if(ClientIsOnline(name)) {
										SendMessageToParticularClient(buffer_to_particular.c_str(), name);
										database.AddMessageToDB(client->name, name, message, "seen");
									} else {
										database.AddMessageToDB(client->name, name, message, "unseen");
									}
								}
							}
						}	
					} else if (receive == 0 || strcmp(buff_out, "exit") == 0){
						sprintf(buff_out, "%s has left", client->name.c_str());
						sysout(buff_out);
						SendMessage(buff_out, client->uid);
						leave_flag = 1;
					} else {
						printf("ERROR: -1\n");
						leave_flag = 1;
					}
					bzero(buff_out, BUFFER_SZ);
					bzero(message, BUFFER_SZ);
					buffer_to_all = "";
					buffer_to_particular = "";
				}
			}
		}
	}
	close(client->sockfd);
    RemoveQueue(client->name);
	delete client;
	pthread_detach(pthread_self());

	return NULL;
}
