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
    void disconnect();
    bool isClientConnected();

private:
    static void* receiveMsg(void* args);

signals:
    void connectedToServer();
    void disconnected();
};

#endif // CLIENT_H
