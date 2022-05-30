#ifndef RUSTY_IMAGERESTORATION_H
#define RUSTY_IMAGERESTORATION_H

#include <QWidget>
#include <opencv2/opencv.hpp>

namespace Ui
{
    class ImageRestoration;
}

class ImageRestoration : public QWidget
{
Q_OBJECT

public:
    explicit ImageRestoration(QWidget *parent = nullptr);

    ~ImageRestoration() override;

private slots:

    void showImage();

private:
    static void wiener(const cv::Mat &image, cv::Mat &restoredImage, double length, double angle, double k);

    Ui::ImageRestoration *ui;

    cv::Mat originImage;
};

#endif // RUSTY_IMAGERESTORATION_H
