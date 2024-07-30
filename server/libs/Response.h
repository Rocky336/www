#include <string>
#include <vector>

class Response{
    public:
        int response_status;
        int content_length;
        std::string content_type;
        std::string headers;
        std::string datastring;
        
        Response();
        Response(int rs, std::string content, std::string type);
};