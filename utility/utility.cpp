#include <QFileDialog>

#include "utility.h"

cv::Mat utility::openImageFile(QWidget *parent)
{
    QString fileName = QFileDialog::getOpenFileName(parent,
                                                    "打开图像",
                                                    ".",
                                                    "图像 (*.jpg *.png)");

    if (fileName.isEmpty())
    {
        cv::Mat emptyImage;
        return emptyImage;
    }

    return cv::imread(fileName.toLocal8Bit().toStdString());
}

void utility::saveImageFile(cv::Mat &image, QWidget *parent)
{
    QString fileName = QFileDialog::getSaveFileName(parent,
                                                    "保存图像",
                                                    ".",
                                                    "JPEG 图像 (*.jpg);;PNG 图像 (*.png)");

    if (fileName.isEmpty())
    {
        return;
    }

    cv::imwrite(fileName.toLocal8Bit().toStdString(), image);
}

cv::VideoCapture *utility::openVideoFile(QWidget *parent)
{
    QString fileName = QFileDialog::getOpenFileName(parent,
                                                    "打开视频",
                                                    ".",
                                                    "视频 (*.mp4 *.flv *.avi *.mkv)");

    if (fileName.isEmpty())
    {
        return new cv::VideoCapture();
    }

    return new cv::VideoCapture(fileName.toLocal8Bit().toStdString());
}

void utility::showCvMatInLabel(cv::Mat &image, QLabel *label)
{
    cv::Mat convertedImage;
    cvtColor(image, convertedImage, cv::COLOR_BGR2RGB);

    QImage qImage(static_cast<uchar *>(convertedImage.data),
                  convertedImage.cols,
                  convertedImage.rows,
                  static_cast<int>(convertedImage.step),
                  QImage::Format_RGB888);

    label->setPixmap(QPixmap::fromImage(qImage).scaled(label->size(),
                                                       Qt::KeepAspectRatio,
                                                       Qt::SmoothTransformation));
}
