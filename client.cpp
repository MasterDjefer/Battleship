#include "client.h"

Client::Client() : NetworkBase()
{
    mClientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    assertError(mClientSocket, "socket create");
}

Client::~Client()
{
    int res = pthread_cancel(mThread);
    if (res != 0)
    {
        cout << "error: thread" << endl;
        exit(EXIT_FAILURE);
    }

    assertError(close(mClientSocket), "close");
}

bool Client::connectToServer()
{
    struct sockaddr_in sockAddr;
    sockAddr.sin_family = AF_INET;
    sockAddr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    sockAddr.sin_port = htons(PORT);

    bool res = ::connect(mClientSocket, (sockaddr*)&sockAddr, sizeof(sockAddr)) == 0;
    if (res)
    {
        char buffer[BUFFER_SIZE];
        memset(buffer, 0, BUFFER_SIZE);
        strcpy(buffer, "hello from the client");

        assertError(send(mClientSocket, buffer, strlen(buffer), 0), "send");
    //    close(mClientSocket);

        pthread_create(&mThread, NULL, (ThreadFunc)&Client::receiveMsg, (void*)this);
        pthread_detach(mThread);
    }

    return res;
}

void* Client::receiveMsg(void* args)
{
    Client* self = (Client*)args;

    char buffer[BUFFER_SIZE];

    while (true)
    {
        memset(buffer, 0, BUFFER_SIZE);
        assertError(recv(self->mClientSocket, buffer, BUFFER_SIZE, 0), "recv");

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
