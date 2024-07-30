#include "Response.h"


Response::Response(){}

Response::Response(int rs, std::string content, std::string type){
    response_status = rs;
    datastring = content;
    content_length = content.length();
    content_type = type;
}