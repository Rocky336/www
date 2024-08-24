#include "Response.h"


Response::Response(){
    headers = "";
}

Response::Response(int rs, std::string content, std::string type){
    response_status = rs;
    datastring = content;
    headers = "";
    content_length = content.length();
    content_type = type;
}