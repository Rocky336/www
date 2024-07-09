g++ -c Client.cpp -o Client.o
g++ -c WindowsHttpServer.cpp -lws2_32 -o WindowsHttpServer.o
ar rvs libs.a Client.o WindowsHttpServer.o