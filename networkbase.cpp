#include "networkbase.h"

NetworkBase::NetworkBase() : mClientSocket(-1), mThread(0)
{
}

NetworkBase::~NetworkBase()
{
    stopThread();
}

void NetworkBase::sendMsg(const char* buffer)
{
    if (mClientSocket == -1)
    {
        cout << "error send msg" << endl;
        exit(EXIT_FAILURE);
    }

    assertError(send(mClientSocket, buffer, strlen(buffer), 0), "send");
}

void NetworkBase::sendDisconnectMsg()
{
    assertError(send(mClientSocket, DISCONNECT_MSG, strlen(DISCONNECT_MSG), 0), "send");
}

void NetworkBase::stopThread()
{
    if(pthread_kill(mThread, 0) == 0)//still running
    {
        int res = pthread_cancel(mThread);
        if (res != 0)
        {
            cout << "error: thread" << endl;
            exit(EXIT_FAILURE);
        }
    }
}

bool NetworkBase::getCoordinates(char *buffer, QPoint &coordinates)
{
    std::string sBuffer = buffer;
    size_t separatorPos = sBuffer.find(';');
    if (separatorPos != std::string::npos)
    {
        coordinates.setX(std::stoi(sBuffer.substr(0, separatorPos)));
        coordinates.setY(std::stoi(sBuffer.substr(separatorPos + 1)));
        return true;
    }

    return false;
}

void NetworkBase::assertError(int value, const char* msg)
{
    if (value == ERROR)
    {
        cout << "error: " << msg << endl;
        exit(EXIT_FAILURE);
    }
}
