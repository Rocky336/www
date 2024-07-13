#include <string>

class Response{
    public:
        int response_status;
        std::string content_type;
        std::string headers;
        char * data;
        
        Response();
};