#ifndef CLIENT_H
#define CLIENT_H

#include "networkbase.h"

class Client : public NetworkBase
{
public:
    Client();
    ~Client();
    bool connectToServer();

private:
    static void* receiveMsg(void* args);
};

#endif // CLIENT_H
