#ifndef SERVER_H
#define SERVER_H

#include "networkbase.h"

class Server : public NetworkBase
{
    Q_OBJECT

    int mServerSock;
    bool mServerRunning;
public:
    Server();
    ~Server();
    void startServer();
    void stopServer();
    bool isServerRunning();

private:
    static void* start(void* args);

signals:
    void connectionAccepted();
};

#endif // SERVER_H
