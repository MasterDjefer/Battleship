#include "client.h"

Client::Client() : NetworkBase(), mClientConnected(false)
{
}

Client::~Client()
{
    disconnect();
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
        mClientConnected = true;

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
    mClientConnected = false;

    if (mClientSocket != -1)
    {
        assertError(close(mClientSocket), "close");
        mClientSocket = -1;
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

bool Client::isClientConnected()
{
    return mClientConnected;
}

void* Client::receiveMsg(void* args)
{
    Client* self = (Client*)args;

    char buffer[BUFFER_SIZE];

    while (true)
    {
        memset(buffer, 0, BUFFER_SIZE);
        assertError(recv(self->mClientSocket, buffer, BUFFER_SIZE, 0), "recv");
        cout << "ERROR MAJOR!!!" << endl;

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
