#ifndef CLIENT_H
#define CLIENT_H

#include "networkbase.h"

class Client : public NetworkBase
{
    Q_OBJECT

public:
    Client();
    ~Client();
    bool connectToServer();

private:
    static void* receiveMsg(void* args);

signals:
    void connectedToServer();
};

#endif // CLIENT_H
