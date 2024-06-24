#include "manageplayer.h"
#include <QPermissions>
#include <QJniObject>
#include <QGuiApplication>
#include <QNetworkInterface>

ManagePlayer::ManagePlayer(QObject *engine, QObject *parent)
    : QObject{parent}
{
    this->frontEnd = engine;
}

void ManagePlayer::inputProcessor(unsigned char code)
{
    this->frame = buildFrame(code);
    if(sender.sendFrame(this->frame, 2) != SUCCESS) {
        QMetaObject::invokeMethod(this->frontEnd, "showError", Q_ARG(QVariant, "Ошибка отправки команды"));
    }
}

unsigned char *ManagePlayer::buildFrame(unsigned char code)
{
    unsigned char* data;
    data = new unsigned char[2];

    switch(code) {
    case 0x01:
        data[0] = 0x01;
        data[1] = 255;
        break;
    case 0x02:
        data[0] = 0x02;
        data[1] = 0;
        break;
    case 0x03:
        data[0] = 0x03;
        data[1] = 0;
        break;
    case 0x04:
        data[0] = 0x04;
        data[1] = 0;
        break;
    }

    return data;
}

void ManagePlayer::initSender()
{
    if(int status = sender.init("192.168.4.1", 10000) != SUCCESS) {
        QMetaObject::invokeMethod(this->frontEnd, "showError", Q_ARG(QVariant, "Ошибка соединения с игроком"));
    } else {
        QMetaObject::invokeMethod(this->frontEnd, "endConnection");
    }
    emit initGameInfoConnection(this->sender.getSocket());
    emit startStreamSignal("http://192.168.4.1/capture");
}
