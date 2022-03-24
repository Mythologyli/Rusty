#ifndef RUSTY_UTILITY_H
#define RUSTY_UTILITY_H

#include <QLabel>
#include <opencv2/opencv.hpp>

namespace utility
{
    cv::Mat openImageFile(QWidget *parent = nullptr);

    void saveImageFile(cv::Mat &image, QWidget *parent = nullptr);

    void showCvMatInLabel(cv::Mat &image, QLabel *label);
}

#endif //RUSTY_UTILITY_H
