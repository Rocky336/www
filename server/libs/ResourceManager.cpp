#include "ResourceManager.h"
#include <sys/types.h>
#include <dirent.h>
#include <fstream>
#include <iostream>

void ResourceManager::recursiveSearch(std::string dir){
    DIR* dirp = opendir(dir.data());
    struct dirent * dp;

    while ((dp = readdir(dirp)) != NULL) {
        if(dp->d_type==16){ //directory
            if(!(dp->d_namlen==2&&dp->d_name[0]=='.'&&dp->d_name[1]=='.'||dp->d_namlen==1&&dp->d_name[0]=='.')) 
                recursiveSearch(dir +dp->d_name +"/");
        }else{
            std::ifstream index(dir + dp->d_name,std::ios::binary);
        
            std::vector<char> bytes(std::istreambuf_iterator<char>(index),(std::istreambuf_iterator<char>()));
            
            content[(dir + dp->d_name)] = bytes;/*
            printf((dir + dp->d_name).data());
            printf("\n");*/
            index.close();
            delete &index;
        }
    }
    closedir(dirp);
    delete dp;
}
/*
void ResourceManager::loadContentTypes(){
    std::ifstream file("./resources/content_types.csv",std::ios::binary);
    std::string line;
    int i;
    while(getline(file,line)){
        contenttypes[line.substr(0,line.find(','))] = line.substr(line.find(',')+2,line.find(',',line.find(',')+2));
    }
}*/

ResourceManager::ResourceManager(){
    //recursiveSearch("E:/www/routes/");
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
    else if(response.content_type=="json") response.content_type = "application/json";
    else response.content_type = "text/plain";

    std::ifstream index("../routes"+route,std::ios::binary);
    std::vector<char> bytes(std::istreambuf_iterator<char>(index),(std::istreambuf_iterator<char>()));
    
    for(i=0;i<bytes.size();i++)
        response.datastring += bytes.at(i);

    if(bytes.size()!=0){
        response.content_length = bytes.size();
        response.response_status = 200;
    }else{
        std::ifstream index("../routes/not_found.html",std::ios::binary);
        std::vector<char> bytes(std::istreambuf_iterator<char>(index),(std::istreambuf_iterator<char>()));
        
        for(i=0;i<bytes.size();i++)
            response.datastring += bytes.at(i);

        response.content_type = "text/html";
        response.content_length = bytes.size();
        response.response_status = 404;
    }

    index.close();
    return response;
}