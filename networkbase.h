#ifndef NETWORKBASE_H
#define NETWORKBASE_H

#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <string.h>
#include <pthread.h>
#include <signal.h>
#include <QObject>
#include <QPoint>
using namespace std;

#ifndef IPPROTO_TCP
#define IPPROTO_TCP 0
#endif
#define PORT 12345
#define BUFFER_SIZE 100
#define ERROR -1

#define DISCONNECT_MSG "DCONN"

class NetworkBase : public QObject
{
    Q_OBJECT

protected:
    typedef void*(*ThreadFunc)(void*);

    enum ConnectionStatus
    {
        Stopped,
        WaitingForClient,
        ConnectionAccepted,
        Connected,
        Disconnected
    };

    pthread_t mThread;
    int mClientSocket;
    ConnectionStatus mConnStatus;
public:
    NetworkBase();
    ~NetworkBase();
    void sendMsg(const char* buffer);
    void sendDisconnectMsg();

protected:
    void stopThread();

protected:
    static bool getCoordinates(char* buffer, QPoint& coordinates);
    static void assertError(int value, const char* msg);

signals:
    void coordinatesReceived(const QPoint& coordinates);
};

#endif // NETWORKBASE_H
