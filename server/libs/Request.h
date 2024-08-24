#include "Params.h"
#include <string>

class Request{
    public:
        int requestType;
        std::string route_string;
        std::string cookies;
        Params params[5];
        
        Request();
};