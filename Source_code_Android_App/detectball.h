 #ifndef DETECTBALL_H
#define DETECTBALL_H

#include <QObject>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <QImage>
#include <vector>

class DetectBall : public QObject
{
    Q_OBJECT
public:
    explicit DetectBall(QObject *parent = nullptr);

    QImage findBall(QByteArray inputImage);

    cv::Mat convertToMat(QByteArray inputImage);

    QImage convertToQImage(cv::Mat inputImage);

signals:
};

#endif // DETECTBALL_H
