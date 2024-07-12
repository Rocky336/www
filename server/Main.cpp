#include <iostream>
#include <fstream>
#include <map>

#ifdef _WIN32
    #include "libs/WindowsHttpServer.h"
#endif

using namespace std;

map<string,const char *> resources;
map<const char*,string> content;

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
}

int main(int argc, char* argv[]){
    resources["/style/style.css"] = "../routes/style/style.css";
    resources["/assets/images/bars.png"] = "../routes/assets/images/bars.png";
    resources["/index.html"] = "../routes/index.html";
    resources["/favicon.ico"] = "../routes/index.html";
    resources["/"] = "../routes/index.html";

    const char ip[] = "127.0.0.1";
    Server server(ip,8080);
    server.startListening();

    string line_buffer;

    for (auto const& x : resources){
        if(content.count(x.second)) continue;

        ifstream index(x.second);
        line_buffer = "";
        while(getline(index,line_buffer)) 
            content[x.second] += line_buffer;
        index.close();
    }

    ifstream index("../routes/not_found.html");
    line_buffer = "";
    while(getline(index,line_buffer)) 
        content["not_found"] += line_buffer;
    index.close();

    string header = "HTTP/2 200\n;\nContent-Type:";
    string bad = "HTTP/2 404\n";

    Client client;

    int i;

    while(server.acceptClient(client)){
        Request req = server.decodeRequest(client);
        string result = "";
        string route = "";
        string contenttype = "";
        i = 0;
        while(i<32&&req.route[i]!=' ') route+=req.route[i++];

        auto res = resources.find(route);

        i = route.length()-1;
        while(i>=0&&route.at(i)!='.')
            contenttype = route.at(i--) + contenttype;

        if(contenttype=="png") contenttype = "image/png";
        else if(contenttype=="ico") contenttype = "image/vnd.microsoft.icon";
        else if(contenttype=="html"||contenttype=="/") contenttype = "text/html";
        else if(contenttype=="css") contenttype = "text/css";
        else if(contenttype=="js") contenttype = "text/javascript";
        else contenttype = "text/plain";
        
        printf(contenttype.data());

        if(res!=resources.end()){
            result += header + contenttype +";\nContent-Length:"+ to_string(strlen(content[res->second].data())) + "\n\n" + content[res->second];
        }else{
            result += bad + to_string(strlen(content["not_found"].data())) + "\n\n" + content["not_found"];
        }
        server.sendData(client,result.data(),strlen(result.data()));
        server.closeClient(client);
        updatecontent();
    }

    return 0;
}