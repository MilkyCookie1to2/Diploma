#ifndef VIDEOCAMCAPTURE_H
#define VIDEOCAMCAPTURE_H

#include <opencv2/videoio.hpp>
#include <QTimer>
#include <QObject>
#include <QImage>
#include <QQuickImageProvider>
#include <QNetworkRequest>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QtMultimedia/QVideoFrame>
#include <QUrl>
#include <detectball.h>
#include <QThread>
#include <QMutex>

class VideoCamCapture: public QQuickImageProvider
{
    Q_OBJECT

public:
    VideoCamCapture(QObject *parent = nullptr);

    QImage requestImage(const QString &id, QSize *size, const QSize &requestedSize) override;



    void captureFrame();
    void processThreadFun();

signals:
    void imageChanged();

public slots:
    void startStream(QString URL);

private slots:
    void replyFinished(QNetworkReply *reply);

private:
    QTimer timeUpdateFrame;
    QImage image;
    QNetworkAccessManager m_networkManager;
    QNetworkReply *m_reply;
    QUrl ipCamera;
    QThread *processThread;
    QList<QByteArray> frames;
    DetectBall detector;
    QMutex mutex;
};

#endif // VIDEOCAMCAPTURE_H
