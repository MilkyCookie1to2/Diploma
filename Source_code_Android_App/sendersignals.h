#ifndef SENDERSIGNALS_H
#define SENDERSIGNALS_H

#include <QObject>
#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include "socket.h"

class SenderSignals :public QObject, public Socket
{
    Q_OBJECT
public:
    explicit SenderSignals(QObject *parent = nullptr);

    int init(QString ip, int port);
    int sendFrame(unsigned char* frame, size_t sizeFrame);

    int getSocket();
};

#endif // SENDERSIGNALS_H
