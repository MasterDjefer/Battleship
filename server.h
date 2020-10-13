#ifndef SERVER_H
#define SERVER_H

#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <string.h>
#include <pthread.h>
#include <signal.h>
using namespace std;

#ifndef IPPROTO_TCP
#define IPPROTO_TCP 0
#endif
#define PORT 12345
#define BUFFER_SIZE 100
#define ERROR -1


class Server
{
    typedef void*(*ThreadFunc)(void*);

    pthread_t mThread;
    int mServerSock;
    int mClientSocket;
public:
    Server();
    ~Server();
    void sendMsg(char* buffer);
    void startServer();

private:
    void* start(void* args);
    static void assertError(int value);
};
//int main()
//{
//    Server* server = new Server;
//    server->startServer();
//    sleep(5);

////    server->sendMsg("LOL");

//    delete server;

//    return 0;
//}

#endif // SERVER_H
