#include "../utility/utility.h"
#include "imagebinarization.h"
#include "ui_imagebinarization.h"

ImageBinarization::ImageBinarization(QWidget *parent) :
        QWidget(parent),
        ui(new Ui::ImageBinarization)
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
                connect(ui->thresholdSpinBox, &QSpinBox::valueChanged, this, &ImageBinarization::showImage);
                showImage();
            });

    connect(ui->saveImageButton, &QPushButton::clicked,
            [&]()
            {
                if (!originImage.empty())
                {
                    cv::Mat binaryImage = convertToBinaryImage(originImage, ui->thresholdSpinBox->value());
                    utility::saveImageFile(binaryImage, this);
                }
            });
}

ImageBinarization::~ImageBinarization()
{
    delete ui;
}

void ImageBinarization::showImage()
{
    cv::Mat binaryImage = convertToBinaryImage(originImage, ui->thresholdSpinBox->value());

    utility::showCvMatInLabel(originImage, ui->originImageLabel);
    utility::showCvMatInLabel(binaryImage, ui->binaryImageLabel);
}

cv::Mat ImageBinarization::convertToBinaryImage(cv::Mat &image, double thresh)
{
    cv::Mat grayImage;
    cvtColor(image, grayImage, cv::COLOR_BGR2GRAY); // BRG to grayscale.

    cv::Mat binaryImage;
    threshold(grayImage, binaryImage, thresh, 255, cv::THRESH_BINARY); // Grayscale to binary.

    return binaryImage;
}
