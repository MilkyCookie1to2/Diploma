#include "gameinfo.h"
#include <QGuiApplication>

GameInfo::GameInfo(QObject *engine, QObject *parent)
    : QObject{parent}
{
    this->frontEnd = engine;
    countLifes = 11;
    QMetaObject::invokeMethod(this->frontEnd, "setCountLifes", Q_ARG(QVariant, QVariant::fromValue(QString::number(countLifes))));
}

int GameInfo::receiveProcess()
{
    struct frameCommand{
        unsigned char code, param;
    } data;

    while(1) {
        if(receiver.recieveData((unsigned char*)&data) == SUCCESS) {
            if(static_cast<int>(data.param) != countLifes) {
                countLifes = static_cast<int>(data.param);
                QMetaObject::invokeMethod(this->frontEnd, "setCountLifes", Q_ARG(QVariant, QVariant::fromValue(QString::number(static_cast<int>(data.param)))));
                if(countLifes == 0) {
                    QMetaObject::invokeMethod(this->frontEnd, "showEmptyLifes");
                }
            }
        }

    }
}

void GameInfo::initReceiver(int socket)
{
    this->receiver.setSocket(socket);
    this->recieveThread = QThread::create(&GameInfo::receiveProcess, this);
    this->recieveThread->start();
}
