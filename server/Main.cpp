#include <iostream>
#include <fstream>
#include <map>

#ifdef _WIN32
    #include "libs/WindowsHttpServer.h"
#endif

using namespace std;
/*
void updatecontent(){
    string line_buffer;

    for (auto const& x : resources){
        ifstream index(x.second,ios_base::binary);
        
        line_buffer = "";
        content[x.second] = "";
        while(getline(index,line_buffer)) 
            content[x.second] += line_buffer;
        index.close();
    }
}*/
string GetLastErrorAsString()
{
    //Get the error message ID, if any.
    DWORD errorMessageID = ::GetLastError();
    if(errorMessageID == 0) {
        return string(); //No error message has been recorded
    }
    
    LPSTR messageBuffer = nullptr;

    //Ask Win32 to give us the string version of that message ID.
    //The parameters we pass in, tell Win32 to create the buffer that holds the message for us (because we don't yet know how long the message string will be).
    size_t size = FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
                                 NULL, errorMessageID, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR)&messageBuffer, 0, NULL);
    
    //Copy the error message into a std::string.
    string message(messageBuffer, size);
    
    //Free the Win32's string's buffer.
    LocalFree(messageBuffer);
            
    return message;
}


int main(int argc, char* argv[]){
    const char ip[] = "127.0.0.1";
    Server server(ip,8080);
    server.startListening();

    string header = "HTTP/1.1 ";

    Client client;

    int i;

    while(server.acceptClient(client)){
        Request req = server.decodeRequest(client);
        Response res = server.generateResponse(client,req);
        
        string response = header+ to_string(res.response_status) + " OK\nContent-Type: "+res.content_type+"\nContent-Length: "+to_string(strlen(res.data))+"\n\n"+res.data;
        
        server.sendData(client,response.data(),response.length());
        server.closeClient(client);
    }

    return 0;
}
