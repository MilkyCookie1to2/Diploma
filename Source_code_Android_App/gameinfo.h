#ifndef GAMEINFO_H
#define GAMEINFO_H

#include <QObject>
#include <receiverdata.h>
#include <QThread>

class GameInfo : public QObject
{
    Q_OBJECT
public:
    explicit GameInfo(QObject *engine, QObject *parent = nullptr);

    int receiveProcess();

public slots:
    void initReceiver(int socket);

signals:
    void showError(QString errorMessage);

private:
    int countLifes;
    ReceiverData receiver;
    QObject *frontEnd;
    QThread *recieveThread;
};

#endif // GAMEINFO_H
