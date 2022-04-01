#ifndef RUSTY_IMAGEBINARIZATION_H
#define RUSTY_IMAGEBINARIZATION_H

#include <QWidget>
#include <opencv2/opencv.hpp>

namespace Ui
{
    class ImageBinarization;
}

class ImageBinarization : public QWidget
{
Q_OBJECT

public:
    explicit ImageBinarization(QWidget *parent = nullptr);

    ~ImageBinarization() override;

private slots:

    void showImage();

private:
    static cv::Mat convertToBinaryImage(cv::Mat &image, double threshold);

    Ui::ImageBinarization *ui;

    cv::Mat originImage;
};

#endif // RUSTY_IMAGEBINARIZATION_H
