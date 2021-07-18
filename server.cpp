#include "server.h"

Server::Server() : NetworkBase(), mServerSock(-1)
{
    mConnStatus = Stopped;
}

Server::~Server()
{
    stopServer();
    stopThread();
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

    pthread_create(&mThread, nullptr, (ThreadFunc)&Server::start, (void*)this);
    pthread_detach(mThread);
}

void Server::stopServer()
{
    if (mConnStatus == ConnectionAccepted)
    {
        assertError(close(mClientSocket), "close");
    }

    emit serverStopped();
    mConnStatus = Stopped;

    assertError(close(mServerSock), "close");
}

bool Server::isServerRunning()
{
    return mConnStatus == WaitingForClient || mConnStatus == ConnectionAccepted;
}

void* Server::start(void* args)
{
    Server* self = (Server*)args;

    struct sockaddr_in clientSockAddr;
    socklen_t clientSockAddrLen = sizeof(clientSockAddr);

    cout << "waiting..." << endl;
    self->mConnStatus = WaitingForClient;
    self->mClientSocket = accept(self->mServerSock, (sockaddr*)&clientSockAddr, &clientSockAddrLen);
    self->mConnStatus = ConnectionAccepted;

    cout << "connection accepted" << endl;

    if (self->mClientSocket == -1)
    {
        cout << "socket was closed" << endl;
        return nullptr;
    }

    emit self->connectionAccepted();

    char buffer[BUFFER_SIZE];
    while (true)
    {
        memset(buffer, 0, BUFFER_SIZE);

        int resRecv = recv(self->mClientSocket, buffer, BUFFER_SIZE, 0);

        if (resRecv == ERROR)
        {
            cout << "error recv" << endl;
            return nullptr;
        }
        if (resRecv == 0)
        {
            cout << "client socket closed: resRecv = 0" << endl;
            self->stopServer();
            return nullptr;
        }

        QPoint coordinates;
        if (getCoordinates(buffer, coordinates))
        {
            //emit signal with coordanites
            emit self->coordinatesReceived(coordinates);
        }
        else
        if (!strcmp(buffer, DISCONNECT_MSG))
        {
            self->stopServer();
            return nullptr;
        }

        cout << buffer << endl;
    }

    return nullptr;
}
