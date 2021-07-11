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

void NetworkBase::assertError(int value, const char* msg)
{
    if (value == ERROR)
    {
        cout << "error: " << msg << endl;
        exit(EXIT_FAILURE);
    }
}
