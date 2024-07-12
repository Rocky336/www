#include <winsock2.h>
#include <map>
#include "Client.h"
#include "Request.h"

struct ClientComparator{
    bool operator() (const Client& lhs, const Client& rhs) const{
        return lhs.id < rhs.id;
    }
};

class Server{
    private:
        SOCKET serversocket;
        std::map<Client, SOCKET, ClientComparator> clientpool;

    public: 
        Server(const char address[], int port);
        bool startListening();
        bool acceptClient(Client client);
        int receiveData(Client client, char buffer[], int length);
        int sendData(Client client, const char buffer[], int length);
        void closeClient(Client client);
        Request decodeRequest(Client client);
        void handleRequest(Client client, Request req);
};