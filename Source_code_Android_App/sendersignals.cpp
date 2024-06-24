#include "sendersignals.h"
#include <QDebug>
 #include <errno.h>

SenderSignals::SenderSignals(QObject *parent)
    : QObject{parent}, Socket{} {

}

int SenderSignals::init(QString ip, int port)
{
    if(initSocket(ip, port) < 0) {
        return ERROR_INIT_SOCKET;
    }

    if (::connect(socketDev, (sockaddr*)&socketinfo, sizeof(socketinfo)) < 0) {
        return ERROR_CONNECTION;
    }

    return SUCCESS;
}

int SenderSignals::sendFrame(unsigned char *frame, size_t sizeFrame)
{
    qDebug() << frame[0] << frame[1];
    if(::send(socketDev, frame, sizeFrame, NULL) < 0) {
        return ERROR_SEND;
    }

    return SUCCESS;
}

int SenderSignals::getSocket()
{
    return this->socketDev;
}
