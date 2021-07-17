#include "server.h"

Server::Server() : NetworkBase(), mServerSock(-1)
{
}

Server::~Server()
{
    stopServer();
}

void Server::startServer()
{
    mServerSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    assertError(mServerSock, "socket create");

    struct sockaddr_in sockAddr;
    sockAddr.sin_family = AF_INET;
    sockAddr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    sockAddr.sin_port = htons(PORT);

    assertError(bind(mServerSock, (sockaddr*)&sockAddr, sizeof(sockAddr)), "bind");
    assertError(listen(mServerSock, 10), "listen");

    pthread_create(&mThread, NULL, (ThreadFunc)&Server::start, (void*)this);
    pthread_detach(mThread);
}

void Server::stopServer()
{
    if (mClientSocket != -1)
    {
        assertError(close(mClientSocket), "close");
        mClientSocket = -1;
    }
    if (mServerSock != -1)
    {
        assertError(close(mServerSock), "close");
        mServerSock = -1;
    }

    if (mThread != 0)
    {
        int res = pthread_cancel(mThread);
        mThread = 0;
        if (res != 0)
        {
            cout << "error: thread" << endl;
            exit(EXIT_FAILURE);
        }
    }
}

void* Server::start(void* args)
{
    Server* self = (Server*)args;

    struct sockaddr_in clientSockAddr;
    socklen_t clientSockAddrLen = sizeof(clientSockAddr);

    cout << "waiting..." << endl;
    self->mClientSocket = accept(self->mServerSock, (sockaddr*)&clientSockAddr, &clientSockAddrLen);
    emit self->connectionAccepted();
    cout << "connection accepted" << endl;

    //assertError(self->mClientSocket, "client socket");
    if (self->mClientSocket == -1)
    {
        cout << "socket was closed" << endl;
        return NULL;
    }

    char buffer[BUFFER_SIZE];
    while (true)
    {
        memset(buffer, 0, BUFFER_SIZE);

        int resRecv = recv(self->mClientSocket, buffer, BUFFER_SIZE, 0);

        if (resRecv == ERROR)
        {
            cout << "error recv" << endl;
            close(self->mClientSocket);
        }
        if (resRecv == 0)
        {
            cout << "client socket closed: resRecv = 0" << endl;
            close(self->mClientSocket);
            return nullptr;
        }

        QPoint coordinates;
        if (getCoordinates(buffer, coordinates))
        {
            //emit signal with coordanites
            emit self->coordinatesReceived(coordinates);
        }

        cout << buffer << endl;
    }

    return nullptr;
}
