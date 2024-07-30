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
    printf(buffer);

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

    int i = 3;                                  
    while(i<1024&&buffer[i]!=' ') i++;
    while(i<1024&&buffer[i]==' ') i++;                  //vado al primo carattere != ' '

    char address[162];
    
    int j = 0;

    while(i<1024&&buffer[i]!=' ')                       //prendo tutto quello che c'è fino al prox spazio e lo salvo in address
        address[j++] = buffer[++i];
    for(;j<162;j++) address[j] = ' ';           //

    i = 0;
    bool value = false;

    while(i<162&&address[i]!='?'&&address[i]!=' ')
        req.route_string += address[i++];

    i++;
    j = 0;

    value = false;

    while(i<162&&address[i]!=' '){
        if(address[i]=='=')
            value = true;
        else if(address[i]=='&'){
            j++;
            value = false;
        }else if(value)
            req.params[j].string_value += address[i];
        else
            req.params[j].param_name += address[i];
        i++;
    }
    
    return req;
}

Response Server::generateResponse(Client client, Request req){
    if(req.route_string.find("..")) return *(new Response(200,"gay","plain/text"));
    if(req.requestType==0){
        if(req.route_string=="/") req.route_string = "/index.html";
        if(endpoints[req.route_string]) return endpoints[req.route_string](req);
        return resourcepool.getResource(req.route_string);
    }
}

void Server::addEndpoint(std::string name,Response (*function)(Request)){
    endpoints[name] = function;
}