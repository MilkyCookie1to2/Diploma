#ifndef SOCKET_H
#define SOCKET_H

#include <QObject>
#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>

#define SUCCESS 0
#define ERROR_INIT_SOCKET -1
#define ERROR_CONNECTION -2
#define ERROR_SEND -3
#define ERROR_RECEIVE -4

class Socket
{
public:
    Socket();

    int initSocket(QString ip, int port);

protected:
    struct sockaddr_in socketinfo;
    int socketDev;
};

#endif // SOCKET_H
