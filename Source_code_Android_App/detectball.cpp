#include "detectball.h"
#include <QDebug>

DetectBall::DetectBall(QObject *parent)
    : QObject{parent}
{}

QImage DetectBall::findBall(QByteArray input)
{
    cv::Mat image = convertToMat(input);

    cv::Mat gray;
    cv::cvtColor(image, gray, cv::COLOR_RGB2GRAY);
    medianBlur(gray, gray, 5);

    std::vector<cv::Vec3f> circles;

    cv::HoughCircles(gray, circles, cv::HOUGH_GRADIENT,  1, 1000, 120, 30, 20, 110);

    for (size_t i = 0; i < circles.size(); i++)
    {
        cv::Vec3i c = circles[i];
        cv::Point center = cv::Point(c[0], c[1]);

        circle(image, center, 1, cv::Scalar(0, 100, 100), 3, cv::LINE_AA);

        int radius = c[2];
        circle(image, center, radius, cv::Scalar(255, 0, 255), 3, cv::LINE_AA);
    }

    return convertToQImage(image);
}

cv::Mat DetectBall::convertToMat(QByteArray inputImage)
{
    // cv::Mat outputImage = cv::Mat(inputImage.height(), inputImage.width(), CV_8UC4,
    //         const_cast<uchar*>(inputImage.bits()),
    //         inputImage.bytesPerLine()).clone();
    std::vector<uchar> data(inputImage.begin(), inputImage.end());
    cv::Mat outputImage = cv::imdecode(data, cv::IMREAD_COLOR);
    return outputImage;
}

QImage DetectBall::convertToQImage(cv::Mat inputImage)
{
    //cv::Mat image;
    //cv::cvtColor(inputImage, image, cv::COLOR_BGR2RGB);
    QImage outputImage = QImage(inputImage.data, inputImage.cols, inputImage.rows, inputImage.step, QImage::Format_RGB888).rgbSwapped();

    return outputImage;
}


