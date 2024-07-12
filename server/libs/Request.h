#include "Params.h"

class Request{
    public:
        int requestType;
        char route[32];
        Params params[5];
        
        Request();
};