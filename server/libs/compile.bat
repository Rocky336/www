g++ -c Client.cpp -o Client.o
g++ -c Params.cpp -o Params.o
g++ -c Request.cpp -o Request.o
g++ -c WindowsHttpServer.cpp -lws2_32 -o WindowsHttpServer.o
ar rvs libs.a Client.o Params.o Request.o WindowsHttpServer.o