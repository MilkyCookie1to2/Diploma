#include "connectiontoplayer.h"
#include <QPermissions>
#include <QJniObject>
#include <QJsonDocument>
#include <QPermission>

ConnectionToPlayer *ConnectionToPlayer::m_instance = nullptr;

ConnectionToPlayer::ConnectionToPlayer(QObject *engine, QObject *parent)
    : QObject{parent}
{
    this->frontEnd = engine;
    m_instance = this;
    javaWiFiApi = QJniObject("org/qtproject/example/pingpongmobile/WiFiApi");

    QJniObject activity = QNativeInterface::QAndroidApplication::context();
    if (activity.isValid()) {
        QJniObject window =
            activity.callObjectMethod("getWindow", "()Landroid/view/Window;");

        if (window.isValid()) {
            const int FLAG_KEEP_SCREEN_ON = 128;
            window.callMethod<void>("addFlags", "(I)V", FLAG_KEEP_SCREEN_ON);
        }
    }
    QJniEnvironment env;
    if (env->ExceptionCheck()) {
        env->ExceptionClear();
    }
}

void ConnectionToPlayer::checkWiFiConnection()
{
    jint wifiState = QJniObject::callStaticMethod<jint>(
        "org/qtproject/example/pingpongmobile/WiFiApi",
        "checkEnableWifi",
        "(Landroid/content/Context;)I",
        QNativeInterface::QAndroidApplication::context());
    if(!wifiState) {
        QMetaObject::invokeMethod(this->frontEnd, "showErrorWiFi");
    } else {
        if(javaWiFiApi.isValid()) {
            javaWiFiApi.callMethod<int>("checkConnection",
                                        "(Landroid/content/Context;)I",
                                        QNativeInterface::QAndroidApplication::context());
        } else {
            //PRINT CRITICAL ERROR
        }
    }
}

void ConnectionToPlayer::connectToPlayer(QString playerBSSID)
{
    QJniObject BSSID = QJniObject::fromString(playerBSSID);
    jint status = javaWiFiApi.callMethod<jint>("connectToPlayer",
        "(Landroid/content/Context;Ljava/lang/String;)I",
        QNativeInterface::QAndroidApplication::context(), BSSID.object<jstring>());
}

void ConnectionToPlayer::wifiScanComplete(const QString &result)
{
    QStringList listBSSIDs = result.split(' ');

    for(QString BSSID : listBSSIDs) {
        if(BSSID == "e0:5a:1b:d1:6d:41" && !avaliablePlayers.contains("e0:5a:1b:d1:6d:41"))
            avaliablePlayers.append(BSSID);
        if(BSSID == "08:3a:f2:aa:6d:09" && !avaliablePlayers.contains("08:3a:f2:aa:6d:09"))
            avaliablePlayers.append(BSSID);
    }

    if(avaliablePlayers.isEmpty())
        QMetaObject::invokeMethod(this->frontEnd, "showNoAvailable");
    else
        QMetaObject::invokeMethod(this->frontEnd, "openListPlayers", Q_ARG(QVariant, QVariant::fromValue(avaliablePlayers)));
}

void ConnectionToPlayer::setStatusConnection(const QString &result)
{
    if(result == "Success") {
        emit initManageConnection();
    } else {
        QMetaObject::invokeMethod(this->frontEnd, "showError", Q_ARG(QVariant, result));
    }
}

void ConnectionToPlayer::openSettingsApp()
{
    QJniObject::callStaticMethod<void>(
        "org/qtproject/example/pingpongmobile/WiFiApi",
        "openPermissionSettings",
        "(Landroid/content/Context;)V",
        QNativeInterface::QAndroidApplication::context());
}

void ConnectionToPlayer::setPermission()
{
    QLocationPermission locationPermission;
    switch (qApp->checkPermission(locationPermission)) {
    case Qt::PermissionStatus::Undetermined:
        qApp->requestPermission(locationPermission, this,
                                &ConnectionToPlayer::setPermission);
        return;
    case Qt::PermissionStatus::Denied:
        QMetaObject::invokeMethod(this->frontEnd, "showPermissionDialog");
        return;
    case Qt::PermissionStatus::Granted:
        return;
    }
}



