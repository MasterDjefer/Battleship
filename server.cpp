#include "server.h"

Server::Server() : mClientSocket(-1)
{
    mServerSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    (mServerSock);

    struct sockaddr_in sockAddr;
    sockAddr.sin_family = AF_INET;
    sockAddr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    sockAddr.sin_port = htons(PORT);

    (bind(mServerSock, (sockaddr*)&sockAddr, sizeof(sockAddr)));
    (listen(mServerSock, 10));
}

Server::~Server()
{
    close(mServerSock);
    pthread_kill(mThread, 0);
}

void Server::sendMsg(char* buffer)
{
    if (mClientSocket == -1)
    {
        cout << "error send msg" << endl;
        exit(EXIT_FAILURE);
    }

    assertError(send(mClientSocket, buffer, strlen(buffer), 0));
}

void Server::startServer()
{
    pthread_create(&mThread, NULL, (ThreadFunc)&Server::start, (void*)this);
    pthread_detach(mThread);
}

void* Server::start(void* args)
{
    (void)args;

    struct sockaddr_in clientSockAddr;
    socklen_t clientSockAddrLen = sizeof(clientSockAddr);

    cout << "waiting..." << endl;
    mClientSocket = accept(mServerSock, (sockaddr*)&clientSockAddr, &clientSockAddrLen);
    cout << "got connection" << endl;
    assertError(mClientSocket);

    char buffer[BUFFER_SIZE];
    while (true)
    {
        memset(buffer, 0, BUFFER_SIZE);

        int resRecv = recv(mClientSocket, buffer, BUFFER_SIZE, 0);

        if (resRecv == ERROR)
        {
            cout << "error recv" << endl;
            close(mClientSocket);
        }
        if (resRecv == 0)
        {
            cout << "client socket closed" << endl;
            close(mClientSocket);
            return nullptr;
        }

        cout << buffer << endl;
    }

    return nullptr;
}

void Server::assertError(int value)
{
    if (value == ERROR)
    {
        cout << "error" << endl;
        exit(EXIT_FAILURE);
    }
}
