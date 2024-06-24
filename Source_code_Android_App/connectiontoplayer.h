#ifndef CONNECTIONTOPLAYER_H
#define CONNECTIONTOPLAYER_H

#include <QObject>
#include <QGuiApplication>
#include <QNetworkInterface>
#include <QUrl>
#include <sendersignals.h>
#include <QProcess>
#include <QPermissions>
#include <QJniObject>

class ConnectionToPlayer : public QObject
{
    Q_OBJECT
public:
    explicit ConnectionToPlayer(QObject *engine, QObject *parent = nullptr);

    Q_INVOKABLE void checkWiFiConnection();
    Q_INVOKABLE void connectToPlayer(QString playerBSSID);

    static ConnectionToPlayer *m_instance;

    static ConnectionToPlayer *instance()
    {
        return m_instance;
    }

    void wifiScanComplete(const QString &result);
    void setStatusConnection(const QString &result);
    Q_INVOKABLE void openSettingsApp();
    Q_INVOKABLE void setPermission();

signals:
    void wifiErrorMsg();
    void initManageConnection();

private:
    QVariantList avaliablePlayers;
    QUrl ipAddressPlayer;
    QObject *frontEnd;
    QJniObject javaWiFiApi;
};

#endif // CONNECTIONTOPLAYER_H
