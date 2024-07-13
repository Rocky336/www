#include <iostream>
#include <fstream>
#include <map>

#ifdef _WIN32
    #include "libs/WindowsHttpServer.h"
#endif

using namespace std;

int main(int argc, char* argv[]){
    const char ip[] = "192.168.1.53";
    Server server(ip,8080);
    server.startListening();

    string header = "HTTP/1.1 ";

    Client client;

    int i;

    while(server.acceptClient(client)){
        Request req = server.decodeRequest(client);
        
        Response res = server.generateResponse(client,req);
        
        string response = header+ to_string(res.response_status) + " OK\nContent-Type: "+res.content_type+"\nContent-Length: "+to_string(res.content_length)+"\n\n"+res.datastring;

        server.sendData(client,response.data(),response.length());
        server.closeClient(client);
    }

    return 0;
}
