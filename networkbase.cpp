#include "networkbase.h"

NetworkBase::NetworkBase() : mClientSocket(-1)
{
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
