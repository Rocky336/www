#include "WindowsHttpServer.h"

Server::Server(const char address[], int port){
    WSADATA wsaData;
    int wsaerr;
    WORD wVersionRequested = MAKEWORD(2, 2);
    wsaerr = WSAStartup(wVersionRequested, &wsaData);

    this->serversocket = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
    sockaddr_in server;
    server.sin_family = AF_INET;
    server.sin_addr.S_un.S_addr = inet_addr(address);
    server.sin_port = htons(port);
    bind(serversocket,reinterpret_cast<SOCKADDR*>(&server),sizeof(server));
}

bool Server::startListening(){
    return listen(serversocket,1)!=SOCKET_ERROR;
}

bool Server::acceptClient(Client cl){
    clientpool[cl] = accept(serversocket,nullptr,nullptr);
    return true;
}

int Server::receiveData(Client client, char buffer[], int length){
    return recv(clientpool[client],buffer,length,0);
}

int Server::sendData(Client client,const char buffer[], int length){
    send(clientpool[client],buffer,length,0);
    return 0;
}

void Server::closeClient(Client client){
    closesocket(clientpool[client]);
}

Request Server::decodeRequest(Client client){
    char buffer[1024];
    receiveData(client,buffer,1024);

    Request req;

    if(buffer[0]=='G')
        req.requestType = 0;                    //get
    else if(buffer[0]=='H')
        req.requestType = 1;                    //head
    else if(buffer[0]=='P'){
        if(buffer[1]=='O') req.requestType = 2;      //post
        else if(buffer[1]=='U') req.requestType = 3; //put
        else if(buffer[1]=='A') req.requestType = 8; //patch
    }else if(buffer[0]=='D')
        req.requestType = 4;                    //delete
    else if(buffer[0]=='C') 
        req.requestType = 5;                    //connect
    else if(buffer[0]=='O')
        req.requestType = 6;                    //options
    else if (buffer[0]=='T')
        req.requestType = 7;                    //trace
/*
    int i = 3;
    while(buffer[i]!=' ') i++;
    i++;
    char route[128];
    int j = 0;
    while(buffer[i]!=' ')
        route[j++] = buffer[++i];
    for(;j<128;j++) route[j] = ' ';
    i = 0;
    j = 0;

    while(route[i]!='?') req.route[i] = route[i++];
    i++;

    bool value = false;
    int k = 0;
    while(route[i]!=' '){
        if(route[i]=='='){
            value = true;
            k = 0;
        }else if(route[i]=='&'){
            j++;
            k = 0;
            value = false;
        }else if(value)
            req.params[j].value[k++] = route[i];
        else
            req.params[j].param[k++] = route[i];
    }

    printf(req.params[0].param);*/

    return req;
}