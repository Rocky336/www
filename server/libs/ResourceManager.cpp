#include "ResourceManager.h"
#include <sys/types.h>
#include <dirent.h>
#include <fstream>
#include <iostream>

void ResourceManager::recursiveSearch(std::string dir){
    DIR* dirp = opendir(dir.data());
    struct dirent * dp;
    std::vector<char> bytes;

    while ((dp = readdir(dirp)) != NULL) {
        if(dp->d_type==16){ //directory
            if(!(dp->d_namlen==2&&dp->d_name[0]=='.'&&dp->d_name[1]=='.'||dp->d_namlen==1&&dp->d_name[0]=='.')) 
                recursiveSearch(dir +dp->d_name +"/");
        }else{
            std::ifstream index(dir + dp->d_name,std::ios::binary);
        
            std::vector<char> bytes(std::istreambuf_iterator<char>(index),(std::istreambuf_iterator<char>()));
            
            content[(dir + dp->d_name)] = bytes;
            printf((dir + dp->d_name).data());
            printf("\n");
            index.close();
        }
    }
    closedir(dirp);
}

ResourceManager::ResourceManager(){
    std::string dir = "E:/www/routes/";
    recursiveSearch(dir);
    printf("\n\n");
}

Response ResourceManager::getResource(std::string route){
    Response response;

    int i = route.length()-1;

    response.content_type = "";
    while(i>=0&&route.at(i)!='.')  response.content_type = route.at(i--) + response.content_type;
    

    if(response.content_type=="png") response.content_type = "image/png";
    else if(response.content_type=="ico") response.content_type = "image/vnd.microsoft.icon";
    else if(response.content_type=="html"||response.content_type=="") response.content_type = "text/html";
    else if(response.content_type=="css") response.content_type = "text/css";
    else if(response.content_type=="js") response.content_type = "text/javascript";
    else response.content_type = "text/plain";

    char * res = &content["E:/www/routes"+route][0];

    if(res!=0){
        response.data = res;
        response.response_status = 200;
    }else{
        response.data = &content["E:/www/routes/not_found.html"][0];
        response.response_status = 404;
    }

    return response;
}