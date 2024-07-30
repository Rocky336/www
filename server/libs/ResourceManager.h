#include <string>
#include <map>
#include <vector>
#include "Response.h"

class ResourceManager{
    private:
        std::map<std::string,std::vector<char>> content;
        //std::map<std::string,std::string> contenttypes;
        void recursiveSearch(std::string dir);
        //void loadContentTypes();
    public:
        ResourceManager();
        Response getResource(std::string route);
};