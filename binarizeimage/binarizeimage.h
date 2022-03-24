#ifndef RUSTY_BINARIZEIMAGE_H
#define RUSTY_BINARIZEIMAGE_H

#include <QWidget>
#include <opencv2/opencv.hpp>

namespace Ui
{
    class BinarizeImage;
}

class BinarizeImage : public QWidget
{
Q_OBJECT

public:
    explicit BinarizeImage(QWidget *parent = nullptr);

    ~BinarizeImage() override;

private slots:

    void displayImage();

private:
    static cv::Mat convertToBinaryImage(cv::Mat &image, double threshold);

    Ui::BinarizeImage *ui;

    cv::Mat originImage;
};

#endif // RUSTY_BINARIZEIMAGE_H
