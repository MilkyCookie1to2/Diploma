#include "receiverdata.h"
#include <QDebug>

ReceiverData::ReceiverData(QObject *parent)
    : QObject{parent}
{}

int ReceiverData::init(QString ip, int port)
{

    if(initSocket(ip, port) < 0) {
        return ERROR_INIT_SOCKET;
    }

    if (::connect(socketDev, (sockaddr*)&socketinfo, sizeof(socketinfo)) < 0) {
        return ERROR_CONNECTION;
    }

    return SUCCESS;
}

int ReceiverData::recieveData(unsigned char* recieveData)
{ 
    int length = ::recv(socketDev, recieveData, sizeof(frameCommand), NULL);
     qDebug() << "RECIEVE" << length <<  recieveData[0] << recieveData[1];
    if(length == -1) {
        return ERROR_RECEIVE;
    } else {
        return SUCCESS;
    }
}

void ReceiverData::setSocket(int socket)
{
    this->socketDev = socket;
}
