#include "socket.h"
#include <QDebug>

Socket::Socket() {}

int Socket::initSocket(QString ip, int port)
{
    socketinfo.sin_family = AF_INET;
    socketinfo.sin_port = htons(port);
    socketinfo.sin_addr.s_addr = inet_addr(ip.toStdString().c_str());

    socketDev = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    if(socketDev < 0)
        return -1;

    return 0;
}
