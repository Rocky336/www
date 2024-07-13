g++ -c Response.cpp -o Response.o
g++ -c ResourceManager.cpp -o ResourceManager.o
g++ -c Client.cpp -o Client.o
g++ -c Params.cpp -o Params.o
g++ -c Request.cpp -o Request.o
g++ -c WindowsHttpServer.cpp -lws2_32 -o WindowsHttpServer.o
ar rvs libs.a Response.o ResourceManager.o Client.o Params.o Request.o WindowsHttpServer.o