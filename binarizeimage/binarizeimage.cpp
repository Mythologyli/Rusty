#include <QFileDialog>

#include "../utility/utility.h"
#include "binarizeimage.h"
#include "ui_binarizeimage.h"

BinarizeImage::BinarizeImage(QWidget *parent) :
        QWidget(parent),
        ui(new Ui::BinarizeImage)
{
    ui->setupUi(this);

    connect(ui->openImageButton, &QPushButton::clicked,
            [&]()
            {
                originImage = utility::openImageFile(this);

                displayImage();
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

    connect(ui->thresholdSpinBox, &QSpinBox::valueChanged, this, &BinarizeImage::displayImage);
}

BinarizeImage::~BinarizeImage()
{
    delete ui;
}

void BinarizeImage::displayImage()
{
    if (!originImage.empty())
    {
        cv::Mat binaryImage = convertToBinaryImage(originImage, ui->thresholdSpinBox->value());

        utility::showCvMatInLabel(originImage, ui->originImageLabel);
        utility::showCvMatInLabel(binaryImage, ui->binaryImageLabel);
    }
}

cv::Mat BinarizeImage::convertToBinaryImage(cv::Mat &image, double thresh)
{
    cv::Mat grayImage(image.size(), CV_8U);
    cvtColor(image, grayImage, cv::COLOR_BGR2GRAY); // BRG to grayscale.

    cv::Mat binaryImage(image.size(), CV_8U);
    threshold(grayImage, binaryImage, thresh, 255, cv::THRESH_BINARY); // Grayscale to binary.

    return binaryImage;
}
