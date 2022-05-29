#include <QMessageBox>

#include "../utility/utility.h"
#include "imagemorphology.h"
#include "ui_imagemorphology.h"

ImageMorphology::ImageMorphology(QWidget *parent) :
        QWidget(parent),
        ui(new Ui::ImageMorphology)
{
    ui->setupUi(this);

    connect(ui->openImageButton, &QPushButton::clicked,
            [&]()
            {
                cv::Mat image = utility::openImageFile(this);

                if (image.empty())
                {
                    return;
                }

                originImage = image;
                showImage();
            });
}

ImageMorphology::~ImageMorphology()
{
    delete ui;
}

void ImageMorphology::showImage()
{
    cv::Mat binaryImage = binarize(originImage);
    utility::showCvMatInLabel(binaryImage, ui->binarizedImageLabel);

    cv::Mat filteredImage = filter(binaryImage);
    utility::showCvMatInLabel(filteredImage, ui->filteredImageLabel);

    cv::Mat skeletonizedImage = skeletonize(filteredImage);
    utility::showCvMatInLabel(skeletonizedImage, ui->skeletonizedImageLabel);
}

cv::Mat ImageMorphology::binarize(cv::Mat &image)
{
    cv::Mat grayImage;
    cvtColor(image, grayImage, cv::COLOR_BGR2GRAY);

    cv::Mat binaryImage;
    threshold(grayImage, binaryImage, 0, 255, cv::THRESH_BINARY | cv::THRESH_OTSU);

    return binaryImage;
}

cv::Mat ImageMorphology::filter(cv::Mat &image)
{
    cv::Mat filteredImage;

    // Do an opening operation to remove noise.
    cv::Mat element = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(3, 3));
    cv::morphologyEx(image, filteredImage, cv::MORPH_OPEN, element);

    // Do a closing operation to fill in holes.
    cv::morphologyEx(filteredImage, filteredImage, cv::MORPH_CLOSE, element);

    return filteredImage;
}

cv::Mat ImageMorphology::skeletonize(cv::Mat &image)
{
    cv::Mat clonedImage = image.clone(); // Make a copy of the image.

    cv::Mat skeletonImage(clonedImage.size(), CV_8UC1, cv::Scalar(0)); // Create a skeletonImage image.
    cv::Mat tempImage; // Create a temporary image.
    cv::Mat erodedImage; // Create an erodedImage image.

    // Create a kernel.
    cv::Mat element = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(3, 3));

    do
    {
        cv::morphologyEx(clonedImage, tempImage, cv::MORPH_OPEN, element); // Do an opening operation.
        cv::bitwise_not(tempImage, tempImage); // Invert the image.
        cv::bitwise_and(clonedImage, tempImage, tempImage);
        cv::bitwise_or(skeletonImage, tempImage, skeletonImage);
        cv::erode(clonedImage, clonedImage, element); // Erode the image.
    } while (cv::countNonZero(clonedImage) != 0); // Loop until the image is empty.

    return skeletonImage;
}
