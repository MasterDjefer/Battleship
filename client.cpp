#include "client.h"

Client::Client() : NetworkBase()
{
    mConnStatus = Disconnected;
}

Client::~Client()
{
    disconnect();
    stopThread();
}

bool Client::connectToServer()
{
    mClientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    assertError(mClientSocket, "socket create");

    struct sockaddr_in sockAddr;
    sockAddr.sin_family = AF_INET;
    sockAddr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    sockAddr.sin_port = htons(PORT);

    bool res = ::connect(mClientSocket, (sockaddr*)&sockAddr, sizeof(sockAddr)) == 0;
    if (res)
    {
        mConnStatus = Connected;

        emit connectedToServer();
        std::cout << "it\'s client!"<< std::endl;

        char buffer[BUFFER_SIZE];
        memset(buffer, 0, BUFFER_SIZE);
        strcpy(buffer, "hello from the client");

        assertError(send(mClientSocket, buffer, strlen(buffer), 0), "send");

        pthread_create(&mThread, NULL, (ThreadFunc)&Client::receiveMsg, (void*)this);
        pthread_detach(mThread);
    }

    return res;
}

void Client::disconnect()
{
    sendDisconnectMsg();
    emit disconnected();
    mConnStatus = Disconnected;
}

bool Client::isClientConnected()
{
    return mConnStatus == Connected;
}

void* Client::receiveMsg(void* args)
{
    Client* self = (Client*)args;

    char buffer[BUFFER_SIZE];

    while (true)
    {
        memset(buffer, 0, BUFFER_SIZE);
        int res = recv(self->mClientSocket, buffer, BUFFER_SIZE, 0);
        if (res == -1 || res == 0)
        {
            cout << "error or server closed connection" << endl;
            emit self->disconnected();
            self->mConnStatus = Disconnected;
            return nullptr;
        }

        QPoint coordinates;
        if (self->getCoordinates(buffer, coordinates))
        {
            //emit signal with coordanites
            emit self->coordinatesReceived(coordinates);
        }

        cout << buffer << endl;
    }

    return nullptr;
}
