#include <string>
#include <pthread.h>
#include "../../Server/TcpListener.h"
#include "../../Models/Client.h"

#define MAX_CLIENTS 100
using namespace std;

void AddToQueue(Client*);
void RemoveQueue(string); 
void SendMessage(const char*, int);
void SendMessageToParticularClient(const char*, const char*);
vector<string> GetOnLineClients(string);
bool ClientIsOnline(string);