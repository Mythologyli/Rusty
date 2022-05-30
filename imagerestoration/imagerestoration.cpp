#include <complex>

#include "../utility/utility.h"
#include "imagerestoration.h"
#include "ui_imagerestoration.h"

ImageRestoration::ImageRestoration(QWidget *parent) :
        QWidget(parent),
        ui(new Ui::ImageRestoration)
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

                cvtColor(image, originImage, cv::COLOR_BGR2GRAY); // BRG to grayscale.

                connect(ui->lengthDoubleSpinBox, &QDoubleSpinBox::valueChanged, this, &ImageRestoration::showImage);
                connect(ui->angleDoubleSpinBox, &QDoubleSpinBox::valueChanged, this, &ImageRestoration::showImage);
                connect(ui->kDoubleSpinBox, &QDoubleSpinBox::valueChanged, this, &ImageRestoration::showImage);

                showImage();
            });
}

ImageRestoration::~ImageRestoration()
{
    delete ui;
}

void ImageRestoration::showImage()
{
    utility::showCvMatInLabel(originImage, ui->originImageLabel);

    cv::Mat restoredImage;
    wiener(originImage,
           restoredImage,
           ui->lengthDoubleSpinBox->value(),
           ui->angleDoubleSpinBox->value(),
           ui->kDoubleSpinBox->value());

    utility::showCvMatInLabel(restoredImage, ui->restoredImageLabel);
}

void ImageRestoration::wiener(const cv::Mat &image, cv::Mat &restoredImage, double length, double angle, double k)
{
    cv::Mat clonedImage;

    // Convert to CV_32FC1.
    image.convertTo(clonedImage, CV_32FC1);
    cv::normalize(clonedImage, clonedImage, 0, 1, cv::NORM_MINMAX);

    // Get the size of the image.
    int width = clonedImage.cols;
    int height = clonedImage.rows;

    // Create the psf.
    cv::Mat psf = cv::Mat::zeros(cv::Size(width, height), CV_32FC1);
    cv::Point center(width / 2, height / 2);
    cv::ellipse(psf, center, cv::Size(0, (int) (length / 2)), 90 - angle, 0, 360, cv::Scalar(255), -1);

    // psf = psd / sum(psf)
    double sum = cv::sum(psf)[0];
    psf /= sum;

    // Do fft for image and psf.
    cv::Mat imageF, psfF;

    cv::dft(clonedImage, imageF, cv::DFT_COMPLEX_OUTPUT);
    cv::dft(psf, psfF, cv::DFT_COMPLEX_OUTPUT);

    // Wiener filter.
    for (int i = 0; i < width; i++)
    {
        for (int j = 0; j < height; j++)
        {
            std::complex<double> G(imageF.at<cv::Vec2f>(i, j)[0], imageF.at<cv::Vec2f>(i, j)[1]);
            std::complex<double> H(psfF.at<cv::Vec2f>(i, j)[0], psfF.at<cv::Vec2f>(i, j)[1]);
            double H2 = std::abs(H) * std::abs(H);
            std::complex<double> res = H2 == 0 ? std::complex<double>{0, 0} : H2 / (H2 + k) / H * G;
            imageF.at<cv::Vec2f>(i, j)[0] = (float) res.real();
            imageF.at<cv::Vec2f>(i, j)[1] = (float) res.imag();
        }
    }

    // Do inverse fft.
    cv::Mat tempImage;
    cv::idft(imageF, tempImage, cv::DFT_SCALE | cv::DFT_REAL_OUTPUT);

    // Shift the image.
    cv::Mat shiftedImage(tempImage.size(), tempImage.type());
    cv::Mat area1 = cv::Mat(tempImage, cv::Rect(0, 0, width / 2, height / 2));
    cv::Mat area2 = cv::Mat(tempImage, cv::Rect(width / 2, 0, width / 2, height / 2));
    cv::Mat area3 = cv::Mat(tempImage, cv::Rect(0, height / 2, width / 2, height / 2));
    cv::Mat area4 = cv::Mat(tempImage, cv::Rect(width / 2, height / 2, width / 2, height / 2));
    area1.copyTo(shiftedImage(cv::Rect(width / 2, height / 2, width / 2, height / 2)));
    area2.copyTo(shiftedImage(cv::Rect(0, height / 2, width / 2, height / 2)));
    area3.copyTo(shiftedImage(cv::Rect(width / 2, 0, width / 2, height / 2)));
    area4.copyTo(shiftedImage(cv::Rect(0, 0, width / 2, height / 2)));

    // Convert to CV_8UC1.
    cv::normalize(shiftedImage, restoredImage, 0, 255, cv::NORM_MINMAX);
    restoredImage.convertTo(restoredImage, CV_8UC1);
}
