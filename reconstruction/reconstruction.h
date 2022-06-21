#ifndef RUSTY_RECONSTRUCTION_H
#define RUSTY_RECONSTRUCTION_H

#include <QWidget>
#include <opencv2/opencv.hpp>

namespace Ui
{
    class Reconstruction;
}

class Reconstruction : public QWidget
{
Q_OBJECT

public:
    explicit Reconstruction(QWidget *parent = nullptr);

    ~Reconstruction() override;

private slots:

    void showResultImage();

private:

    static cv::Mat getDisparityImage(cv::Mat &imageA, cv::Mat &imageB);

    Ui::Reconstruction *ui;

    cv::Mat leftImage;
    cv::Mat rightImage;
};

#endif // RUSTY_RECONSTRUCTION_H
