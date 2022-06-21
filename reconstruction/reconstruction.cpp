#include <QDebug>
#include <QtConcurrent>
#include <QMessageBox>

#include "../utility/utility.h"
#include "reconstruction.h"
#include "ui_reconstruction.h"

Reconstruction::Reconstruction(QWidget *parent) :
        QWidget(parent),
        ui(new Ui::Reconstruction)
{
    ui->setupUi(this);

    connect(ui->openLeftImageButton, &QPushButton::clicked,
            [&]()
            {
                cv::Mat image = utility::openImageFile(this);

                if (image.empty())
                {
                    return;
                }

                cvtColor(image, leftImage, cv::COLOR_BGR2GRAY); // BRG to grayscale.
                utility::showCvMatInLabel(leftImage, ui->leftImageLabel);
            });

    connect(ui->openRightImageButton, &QPushButton::clicked,
            [&]()
            {
                cv::Mat image = utility::openImageFile(this);

                if (image.empty())
                {
                    return;
                }

                cvtColor(image, rightImage, cv::COLOR_BGR2GRAY); // BRG to grayscale.
                utility::showCvMatInLabel(rightImage, ui->rightImageLabel);
            });

    connect(ui->getResultButton, &QPushButton::clicked,
            [&]()
            {
                if (leftImage.empty() || rightImage.empty())
                {
                    QMessageBox::warning(this, "错误", "请先打开两张图片！");
                    return;
                }

                if (leftImage.size() != rightImage.size())
                {
                    QMessageBox::warning(this, "错误", "两张图片的大小不一致！");
                    return;
                }

                ui->disparityImageLabel->setText("正在计算中...");

                showResultImage();
            });
}

Reconstruction::~Reconstruction()
{
    delete ui;
}

void Reconstruction::showResultImage()
{
    QFuture<void> future = QtConcurrent::run([&]()
                                             {
                                                 cv::Mat disparityImage = getDisparityImage(leftImage, rightImage);

                                                 utility::showCvMatInLabel(disparityImage, ui->disparityImageLabel);
                                             });
}

cv::Mat Reconstruction::getDisparityImage(cv::Mat &imageA, cv::Mat &imageB)
{
    int width = imageA.cols;
    int height = imageA.rows;
    int max_offset = 10;
    int kernel_size = 3;

    cv::Mat disparity(height, width, CV_8UC1);
    cv::Mat maxNcc(height, width, CV_32FC1);
    maxNcc.setTo(0.0);

    for (int offset = 1; offset <= max_offset; offset++)
    {
        cv::Mat shiftedImage(height, width, CV_8UC1);

        // Circle shift the right image.
        for (int i = 0; i < height; i++)
        {
            for (int j = 0; j < width; j++)
            {
                int j2 = j - offset;
                if (j2 < 0)
                {
                    j2 += width;
                }

                shiftedImage.at<uchar>(i, j) = imageB.at<uchar>(i, j2);
            }
        }

        // Calculate NCC.
        for (int y = 0; y < height; y++)
        {
            for (int x = 0; x < width; x++)
            {
                int x1 = std::max(0, x - kernel_size);
                int y1 = std::max(0, y - kernel_size);
                int x2 = std::min(width - 1, x + kernel_size);
                int y2 = std::min(height - 1, y + kernel_size);

                cv::Mat kernelImageA = imageA(cv::Rect(x1, y1, x2 - x1 + 1, y2 - y1 + 1));
                cv::Mat kernelShiftedImage = shiftedImage(cv::Rect(x1, y1, x2 - x1 + 1, y2 - y1 + 1));

                cv::Mat result;
                cv::matchTemplate(kernelImageA, kernelShiftedImage, result, cv::TM_CCORR_NORMED);

                float ncc = result.at<float>(0, 0);

                if (ncc > maxNcc.at<float>(y, x))
                {
                    maxNcc.at<float>(y, x) = ncc;
                    disparity.at<uchar>(y, x) = (uchar) offset;
                }
            }
        }

        qDebug() << "Current offset: " << offset;
    }

    // Normalize the disparity image.
    cv::normalize(disparity, disparity, 0, 255, cv::NORM_MINMAX, CV_8UC1);

    return disparity;
}
