#include "Request.h"

Request::Request(){
    int i;
    for(i = 0; i<32;i++)
        route[i] = ' ';
    requestType = -1;
}