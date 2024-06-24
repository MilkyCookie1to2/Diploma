#ifndef MANAGEPLAYER_H
#define MANAGEPLAYER_H

#include <QObject>
#include <QDebug>
#include <sendersignals.h>

class ManagePlayer : public QObject
{
    Q_OBJECT
public:
    explicit ManagePlayer(QObject *engine, QObject *parent = nullptr);

    Q_INVOKABLE void inputProcessor(unsigned char code);
    unsigned char* buildFrame(unsigned char code);

public slots:
    void initSender();

signals:
    void showError(QString errorMessage);
    void startStreamSignal(QString URL);
    void initGameInfoConnection(int socket);

private:
    unsigned char* frame;
    SenderSignals sender;
    QObject *frontEnd;
};

#endif // MANAGEPLAYER_H
