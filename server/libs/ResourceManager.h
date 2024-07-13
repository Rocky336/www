#include <string>
#include <map>
#include <vector>
#include "Response.h"

class ResourceManager{
    private:
        std::map<std::string,std::vector<char>> content;
        void recursiveSearch(std::string dir);
    public:
        ResourceManager();
        Response getResource(std::string route);
};