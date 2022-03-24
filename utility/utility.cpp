#include <QFileDialog>

#include "utility.h"

cv::Mat utility::openImageFile(QWidget *parent)
{
    QString fileName = QFileDialog::getOpenFileName(parent,
                                                    "打开图像",
                                                    ".",
                                                    "图像 (*.jpg *.png)");

    return cv::imread(fileName.toLocal8Bit().toStdString());
}

void utility::saveImageFile(cv::Mat &image, QWidget *parent)
{
    QString fileName = QFileDialog::getSaveFileName(parent,
                                                    "保存图像",
                                                    ".",
                                                    "JPEG 图像 (*.jpg);;PNG 图像 (*.png)");

    cv::imwrite(fileName.toLocal8Bit().toStdString(), image);
}

void utility::showCvMatInLabel(cv::Mat &image, QLabel *label)
{
    cv::Mat convertedImage;
    cvtColor(image, convertedImage, cv::COLOR_BGR2RGB);

    QImage qImage((uchar *) convertedImage.data,
                  convertedImage.cols,
                  convertedImage.rows,
                  (int) convertedImage.step,
                  QImage::Format_RGB888);

    label->setPixmap(QPixmap::fromImage(qImage).scaled(label->size(),
                                                       Qt::KeepAspectRatio,
                                                       Qt::SmoothTransformation));
}
