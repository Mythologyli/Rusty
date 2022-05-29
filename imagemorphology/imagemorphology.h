#ifndef RUSTY_IMAGEMORPHOLOGY_H
#define RUSTY_IMAGEMORPHOLOGY_H

#include <QWidget>
#include <opencv2/opencv.hpp>


namespace Ui
{
    class ImageMorphology;
}

class ImageMorphology : public QWidget
{
Q_OBJECT

public:
    explicit ImageMorphology(QWidget *parent = nullptr);

    ~ImageMorphology() override;

private slots:

    void showImage();

private:
    static cv::Mat binarize(cv::Mat &image);

    static cv::Mat filter(cv::Mat &image);

    static cv::Mat skeletonize(cv::Mat &image);

    Ui::ImageMorphology *ui;

    cv::Mat originImage;
};

#endif // RUSTY_IMAGEMORPHOLOGY_H
