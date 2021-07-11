#ifndef SERVER_H
#define SERVER_H

#include "networkbase.h"

class Server : public NetworkBase
{
    Q_OBJECT
    int mServerSock;
public:
    Server();
    ~Server();
    void startServer();

private:
    static void* start(void* args);
    static bool getCoordinates(char* buffer, QPoint& coordinates);

signals:
    void connectionAccepted();
};

#endif // SERVER_H