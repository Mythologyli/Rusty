#ifndef RUSTY_IMAGESEGMENTATION_H
#define RUSTY_IMAGESEGMENTATION_H

#include <QWidget>
#include <opencv2/opencv.hpp>

namespace Ui
{
    class ImageSegmentation;
}

typedef struct
{
    std::vector<cv::Point> points;
    std::vector<int> chainCode;
} BinaryEdge;

class ImageSegmentation : public QWidget
{
Q_OBJECT

public:
    explicit ImageSegmentation(QWidget *parent = nullptr);

    ~ImageSegmentation() override;

private slots:

    void showImage();

private:
    static cv::Mat convertImage(cv::Mat &image, double thresh);

    static double getOtsuThreshold(cv::Mat &image);

    BinaryEdge getBinaryImageEdge(cv::Mat &image) const;

    Ui::ImageSegmentation *ui;

    cv::Mat originImage;
    int foregroundColorValue;
};

#endif // RUSTY_IMAGESEGMENTATION_H
