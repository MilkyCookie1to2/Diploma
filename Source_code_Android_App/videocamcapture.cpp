#include "videocamcapture.h"
#include <QtMultimedia/QMediaPlayer>

VideoCamCapture::VideoCamCapture(QObject *parent) : QQuickImageProvider(QQuickImageProvider::Image) {
    image = QImage(200,200,QImage::Format_RGB888);
    image.fill(QColor("black"));

    connect(&timeUpdateFrame,&QTimer::timeout,this,&VideoCamCapture::captureFrame);
    connect(&m_networkManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(replyFinished(QNetworkReply*)));
}

QImage VideoCamCapture::requestImage(const QString &id, QSize *size, const QSize &requestedSize)
{
    Q_UNUSED(id);

    if(size){
        *size = image.size();
    }

    if(requestedSize.width() > 0 && requestedSize.height() > 0) {
        image = image.scaled(requestedSize.width(), requestedSize.height(), Qt::KeepAspectRatio);
    }

    return image;
}

void VideoCamCapture::startStream(QString URL) {
    this->processThread = QThread::create(&VideoCamCapture::processThreadFun, this);
    this->processThread->start();

    ipCamera.setUrl(URL);
    captureFrame();
    this->timeUpdateFrame.start(1000/60);
}

void VideoCamCapture::captureFrame()
{
    QNetworkRequest request(ipCamera);
    m_networkManager.get(request);
}

void VideoCamCapture::processThreadFun()
{
    while(1) {
        if(!frames.isEmpty()) {
            QByteArray data;
            if(frames.size() > 60) {
                mutex.lock();
                data = frames.last();
                frames.clear();
                mutex.unlock();
            } else {
                mutex.lock();
                data = frames.first();
                frames.removeFirst();
                mutex.unlock();
            }
            //QPixmap pm;
            //pm.loadFromData(data);
            //QImage capturingImage = pm.toImage();
            QImage processedImage = detector.findBall(data);
            this->image = processedImage;

            emit imageChanged();
        }
    }
}

void VideoCamCapture::replyFinished(QNetworkReply *reply)
{
    if (reply->error() == QNetworkReply::NoError) {
        QByteArray data = reply->readAll();
        mutex.lock();
        frames.append(data);
        mutex.unlock();
    }
}

