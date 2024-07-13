#include "Params.h"
#include <string>

class Request{
    public:
        int requestType;
        std::string route_string;
        Params params[5];
        
        Request();
};