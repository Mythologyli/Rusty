#include <QMessageBox>

#include "../utility/utility.h"
#include "imagesegmentation.h"
#include "ui_imagesegmentation.h"

ImageSegmentation::ImageSegmentation(QWidget *parent) :
        QWidget(parent),
        ui(new Ui::ImageSegmentation)
{
    ui->setupUi(this);

    foregroundColorValue = 255;

    connect(ui->openImageButton, &QPushButton::clicked,
            [&]()
            {
                cv::Mat image = utility::openImageFile(this);

                if (image.empty())
                {
                    return;
                }

                cv::Mat grayImage;
                cvtColor(image, grayImage, cv::COLOR_BGR2GRAY); // BRG to grayscale.

                originImage = grayImage;

                connect(ui->foregroundColorComboBox, &QComboBox::currentTextChanged, [&](const QString &text)
                {
                    if (text == "白色")
                    {
                        foregroundColorValue = 255;
                    }
                    else
                    {
                        foregroundColorValue = 0;
                    }

                    showImage();
                });

                showImage();
            });
}

ImageSegmentation::~ImageSegmentation()
{
    delete ui;
}

void ImageSegmentation::showImage()
{
    cv::Mat binaryImage = convertImage(originImage, getOtsuThreshold(originImage));

    BinaryEdge edge = getBinaryImageEdge(binaryImage);

    cv::Mat colorImage;
    cv::cvtColor(binaryImage, colorImage, cv::COLOR_GRAY2BGR);

    for (auto point: edge.points) // Draw points in edge.
    {
        cv::circle(colorImage, point, 1, cv::Scalar(0, 0, 255), -1);
    }

    QString chainCodeText = "";
    for (auto singleCode: edge.chainCode)
    {
        chainCodeText += QString::number(singleCode) + " ";
    }

    if (chainCodeText.isEmpty())
    {
        chainCodeText = "无，请尝试在下方切换前景颜色";
    }

    ui->chainCodeTextBrowser->setText(chainCodeText);

    utility::showCvMatInLabel(originImage, ui->originImageLabel);
    utility::showCvMatInLabel(colorImage, ui->binaryImageLabel);
}

cv::Mat ImageSegmentation::convertImage(cv::Mat &image, double thresh)
{
    cv::Mat noiseReducedImage;
    GaussianBlur(image, noiseReducedImage, cv::Size(3, 3), 0, 0);

    cv::Mat binaryImage;
    threshold(noiseReducedImage, binaryImage, thresh, 255, cv::THRESH_BINARY);

    return binaryImage;
}

double ImageSegmentation::getOtsuThreshold(cv::Mat &image)
{
    cv::Mat noiseReducedImage;
    GaussianBlur(image, noiseReducedImage, cv::Size(3, 3), 0, 0);

    long long grayscaleStat[256] = {0};
    for (int i = 0; i < image.rows; i++)
    {
        for (int j = 0; j < image.cols; j++)
        {
            grayscaleStat[image.at<uchar>(i, j)]++;
        }
    }

    int grayscaleSet[256];
    long long pixelCount = image.rows * image.cols;
    double sum = 0.0;
    for (int i = 0; i < 256; i++)
    {
        sum += static_cast<double>(grayscaleStat[i]) / static_cast<double>(pixelCount);
        grayscaleSet[i] = static_cast<int>(sum * 255.0);
    }

    double grayscaleProbability[256];
    double pk[256];
    double mk[256];
    double pkSum = 0.0;
    double mkSum = 0.0;
    for (int i = 0; i < 256; i++)
    {
        grayscaleProbability[i] = static_cast<double>(grayscaleStat[i]) / static_cast<double>(pixelCount);

        pk[i] = pkSum + grayscaleProbability[i];
        pkSum = pk[i];

        mk[i] = mkSum + i * grayscaleProbability[i];
        mkSum = mk[i];
    }

    int thresh;
    double currentVariance;
    double maxVariance = 0;
    for (int value = 0; value < 256; value++)
    {
        currentVariance = (mkSum * pk[value] - mk[value]) * (mkSum * pk[value] - mk[value]) /
                          (pk[value] * (1 - pk[value]));

        if (currentVariance > maxVariance)
        {
            maxVariance = currentVariance;
            thresh = value;
        }
    }

    return thresh;
}

BinaryEdge ImageSegmentation::getBinaryImageEdge(cv::Mat &image) const
{
    BinaryEdge edge;
    std::vector<cv::Point> points;
    std::vector<int> chainCode;

    for (int i = 0; i < image.rows; i++)
    {
        for (int j = 0; j < image.cols; j++)
        {
            if (image.at<uchar>(i, j) == foregroundColorValue)
            {
                points.emplace_back(cv::Point(j, i));

                int startDirection = 5;
                cv::Point currentPoint = points[0];

                do
                {
                    int correctDirection = -1;
                    int currentDirection = startDirection;
                    for (int searchTimes = 0; searchTimes < 8; searchTimes++)
                    {
                        cv::Point checkPoint;
                        switch (currentDirection)
                        {
                            case 0:
                                checkPoint = cv::Point(currentPoint.x + 1, currentPoint.y);
                                break;

                            case 1:
                                checkPoint = cv::Point(currentPoint.x + 1, currentPoint.y - 1);
                                break;

                            case 2:
                                checkPoint = cv::Point(currentPoint.x, currentPoint.y - 1);
                                break;

                            case 3:
                                checkPoint = cv::Point(currentPoint.x - 1, currentPoint.y - 1);
                                break;

                            case 4:
                                checkPoint = cv::Point(currentPoint.x - 1, currentPoint.y);
                                break;

                            case 5:
                                checkPoint = cv::Point(currentPoint.x - 1, currentPoint.y + 1);
                                break;

                            case 6:
                                checkPoint = cv::Point(currentPoint.x, currentPoint.y + 1);
                                break;

                            case 7:
                                checkPoint = cv::Point(currentPoint.x + 1, currentPoint.y + 1);
                                break;

                            default:
                                break;
                        }

                        if (checkPoint.x < 0 || checkPoint.x >= image.cols ||
                            checkPoint.y < 0 || checkPoint.y >= image.rows)
                        {
                            break;
                        }

                        if (image.at<uchar>(checkPoint.y, checkPoint.x) == foregroundColorValue)
                        {
                            correctDirection = currentDirection;
                            currentPoint = checkPoint;
                            points.push_back(currentPoint);
                            chainCode.push_back(correctDirection);
                            break;
                        }

                        currentDirection = (currentDirection + 1) % 8;
                    }

                    if (correctDirection == -1) // Cannot find the next point
                    {
                        break;
                    }

                    if (correctDirection % 2 == 1)
                    {
                        startDirection = (correctDirection + 6) % 8;
                    }
                    else
                    {
                        startDirection = (correctDirection + 7) % 8;
                    }
                } while (currentPoint != points[0]);

                edge.points = points;
                edge.chainCode = chainCode;

                return edge;
            }
        }
    }

    return edge;
}
