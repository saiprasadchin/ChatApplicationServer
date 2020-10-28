#include "OnLineDAO.h"

Client *clients[MAX_CLIENTS];
static int cli_count = 0;
pthread_mutex_t clients_mutex = PTHREAD_MUTEX_INITIALIZER;

vector<string> GetOnLineClients(string name) {
	pthread_mutex_lock(&clients_mutex);
	vector<string> online_clients;
	for(int i=0; i < MAX_CLIENTS; ++i) {
		if(clients[i]) {
			if(clients[i]->name != name ) {
				online_clients.push_back(clients[i]->name);
			}
		}
	}
	pthread_mutex_unlock(&clients_mutex);
	return online_clients;

}

void AddToQueue(Client* client) {
	pthread_mutex_lock(&clients_mutex);
	for(int i=0; i < MAX_CLIENTS; ++i) {
		if(!clients[i]){
			clients[i] = client;
			cli_count++;
			break;
		}
	}
	pthread_mutex_unlock(&clients_mutex);
}

void RemoveQueue(string name) {
	pthread_mutex_lock(&clients_mutex);
	for(int i=0; i < MAX_CLIENTS; ++i){
		if(clients[i]){
			if(clients[i]->name == name){
				clients[i] = NULL;
				cli_count++;
				break;
			}
		}
	}
	pthread_mutex_unlock(&clients_mutex);
}

void SendMessage(const char* s, int uid) {
	pthread_mutex_lock(&clients_mutex);
	for(int i=0; i < MAX_CLIENTS; ++i){
		if(clients[i]){
			if(clients[i]->uid != uid){
				if(write(clients[i]->sockfd, s, strlen(s)) < 0) {
					perror("ERROR: write to descriptor failed");
					break;
				}
			}
		}
	}
	pthread_mutex_unlock(&clients_mutex);
}

void SendMessageToParticularClient(const char* message,const char* name) {
	pthread_mutex_lock(&clients_mutex);
	for(int i=0; i < MAX_CLIENTS; ++i) {
		if(clients[i]){
			if ((clients[i]->name.compare(name)) == 0 ) {
				ErrorCheck(write(clients[i]->sockfd, message, strlen(message)),
						"ERROR: write to descriptor failed");
			}
		}
	}
	pthread_mutex_unlock(&clients_mutex);
}

bool ClientIsOnline(string receiver_name) {
	bool flag = false;
	pthread_mutex_lock(&clients_mutex);
	for(int i=0; i < MAX_CLIENTS; ++i) {
		if(clients[i]){
			if ((clients[i]->name.compare(receiver_name)) == 0 ) {
				flag = true;
				break;
			}
		}
	}
	pthread_mutex_unlock(&clients_mutex);
	return flag;
}
