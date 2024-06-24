#ifndef RECEIVERDATA_H
#define RECEIVERDATA_H

#include <QObject>
#include <socket.h>

struct frameCommand{
    unsigned char code, param;
};

class ReceiverData : public QObject, public Socket
{
    Q_OBJECT
public:
    explicit ReceiverData(QObject *parent = nullptr);

    int init(QString ip, int port);
    int recieveData(unsigned char* recieveData);
    void setSocket(int socket);

signals:

};

#endif // RECEIVERDATA_H
