#include <iostream>
#include <fstream>
#include <map>

#ifdef _WIN32
    #include "libs/WindowsHttpServer.h"
#endif

using namespace std;
int main(int argc, char* argv[]){
    const char ip[] = "127.0.0.1";
    Server server(ip,8080);
    server.startListening();

    string index_content = "";
    string line_buffer;

    ifstream index("../routes/index.html");
    while(getline(index,line_buffer)) index_content+=line_buffer;
    index.close();

    int index_length = strlen(index_content.data());

    string header = "HTTP/2 200\nContent-Type: text/html;\nContent-Length:"+to_string(index_length)+"\n;charset=utf-8\n;httponly\n\n";

    string result = header+index_content;
    
    Client client;
    while(server.acceptClient(client)){
        server.decodeRequest(client);
        server.sendData(client,result.data(),strlen(result.data()));
        server.closeClient(client);
    }

    return 0;
}