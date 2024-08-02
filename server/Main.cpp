#include <iostream>
#include <fstream>
#include <map>

#ifdef _WIN32
    #include "libs/WindowsHttpServer.h"
#endif

using namespace std;

Response gayFunction(Request req){
    Response r;
    r.content_type = "text/plain";
    r.datastring = "homo";
    r.response_status = 200;
    r.content_length = 4;
    return r;
}

void handleExit(){
    DWORD errorMessageID = ::GetLastError();
    if(errorMessageID == 0) {
        return;
    }
    
    LPSTR messageBuffer = nullptr;
    size_t size = FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
                                 NULL, errorMessageID, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR)&messageBuffer, 0, NULL);
    
    std::string message(messageBuffer, size);
    
    LocalFree(messageBuffer);
    ofstream file("log.txt");
    file<<message;
    file.close();
}

int main(int argc, char* argv[]){
    const char ip[] = "192.168.1.53";
    Server server(ip,80);
    server.startListening();

    string header = "HTTP/1.1 ";

    Client client;

    int i;

    server.addEndpoint("/gay",gayFunction);

    while(server.acceptClient(client)){
        Request req = server.decodeRequest(client);
        
        Response res = server.generateResponse(client,req);
        
        string response = header+ to_string(res.response_status) + " OK\nContent-Type: "+res.content_type+"\nContent-Length: "+to_string(res.content_length)+"\n\n"+res.datastring;

        server.sendData(client,response.data(),response.length());
        server.closeClient(client);
    }

    std::atexit(handleExit);

    return 0;
}
