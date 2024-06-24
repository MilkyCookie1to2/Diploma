#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <opencv2/core/core.hpp>
#include <opencv2/core/mat.hpp>
#include <opencv2/videoio.hpp>
#include <QPermissions>
#include "videocamcapture.h"
#include "connectiontoplayer.h"
#include "manageplayer.h"
#include "sendersignals.h"
#include "gameinfo.h"


#ifdef Q_OS_ANDROID
//Register special Java callback functions immediately once the app loads because they must be executed if the app is started via external Android intents

static void wifiScanComplete(JNIEnv *env, jobject thiz, jstring text)
{
    ConnectionToPlayer::instance()->wifiScanComplete(env->GetStringUTFChars(text, nullptr));
}

static void statusConnection(JNIEnv *env, jobject thiz, jstring text)
{
    ConnectionToPlayer::instance()->setStatusConnection(env->GetStringUTFChars(text, nullptr));
}


static JNINativeMethod methods[] = {
    { "wifiScanComplete", // const char* function name;
        "(Ljava/lang/String;)V", // const char* function signature
        (void *)wifiScanComplete // function pointer
    },
    { "statusConnection", // const char* function name;
        "(Ljava/lang/String;)V", // const char* function signature
        (void *)statusConnection // function pointer
    }
};


JNIEXPORT jint JNI_OnLoad(JavaVM* vm, void* /*reserved*/)
{
    JNIEnv* env;
    // get the JNIEnv pointer.
    if (vm->GetEnv(reinterpret_cast<void**>(&env), JNI_VERSION_1_6) != JNI_OK)
        return JNI_ERR;

    // step 3
    // search for Java class which declares the native methods
    jclass javaClass = env->FindClass("org/qtproject/example/pingpongmobile/WiFiApi");
    if (!javaClass)
        return JNI_ERR;

    // step 4
    // register our native methods
    if (env->RegisterNatives(javaClass, methods, sizeof(methods) / sizeof(methods[0])) < 0)
        return JNI_ERR;

    return JNI_VERSION_1_6;
}
#endif


int main(int argc, char *argv[])
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;

    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreated,
        &app,
        [url](QObject *obj, const QUrl &objUrl) {
            if (!obj && url == objUrl)
                QCoreApplication::exit(-1);
        },
        Qt::QueuedConnection);
    engine.load(url);


    VideoCamCapture *streamProvider(new VideoCamCapture);
    ManagePlayer *inputPlayer(new ManagePlayer(engine.rootObjects().at(0)));
    ConnectionToPlayer *connectionManager(new ConnectionToPlayer(engine.rootObjects().at(0)));
    GameInfo *gameInfo(new GameInfo(engine.rootObjects().at(0)));

    QObject::connect(connectionManager, SIGNAL(initManageConnection()), inputPlayer, SLOT(initSender()));
    QObject::connect(inputPlayer, SIGNAL(initGameInfoConnection(int)), gameInfo, SLOT(initReceiver(int)));
    QObject::connect(inputPlayer, SIGNAL(startStreamSignal(QString)), streamProvider, SLOT(startStream(QString)));

    engine.rootContext()->setContextProperty("streamProvider",streamProvider);
    engine.rootContext()->setContextProperty("inputPlayer",inputPlayer);
    engine.rootContext()->setContextProperty("connectionManager", connectionManager);
    engine.rootContext()->setContextProperty("gameInfo", gameInfo);
    engine.addImageProvider("live", streamProvider);

    connectionManager->setPermission();
    connectionManager->checkWiFiConnection();

    return app.exec();
}
