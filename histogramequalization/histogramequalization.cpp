#include <QLineSeries>
#include <QAreaSeries>

#include "../utility/utility.h"
#include "histogramequalization.h"
#include "ui_histogramequalization.h"

HistogramEqualization::HistogramEqualization(QWidget *parent) :
        QWidget(parent),
        ui(new Ui::HistogramEqualization)
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

                cv::Mat originImage;
                cvtColor(image, originImage, cv::COLOR_BGR2GRAY);

                utility::showCvMatInLabel(originImage, ui->originImageLabel);

                showGrayHistogram(originImage, ui->originHistogramChart);

                cv::Mat equalizedImage = equalizeGrayImage(originImage);

                utility::showCvMatInLabel(equalizedImage, ui->equalizedImageLabel);

                showGrayHistogram(equalizedImage, ui->equalizedHistogramChart);
            });
}

HistogramEqualization::~HistogramEqualization()
{
    delete ui;
}

void HistogramEqualization::showGrayHistogram(const cv::Mat &image, QChartView *chartView)
{
    long long grayscaleStat[256] = {0};
    for (int i = 0; i < image.rows; i++)
    {
        for (int j = 0; j < image.cols; j++)
        {
            grayscaleStat[image.at<uchar>(i, j)]++;
        }
    }

    auto *upperSeries = new QLineSeries();
    auto *lowerSeries = new QLineSeries();

    for (int i = 0; i < 256; i++)
    {
        upperSeries->append(i, static_cast<double>(grayscaleStat[i]));
        lowerSeries->append(i, 0);
    }

    auto *series = new QAreaSeries(upperSeries, lowerSeries);
    series->setColor(QColor(Qt::cyan));

    chartView->chart()->removeAllSeries();
    chartView->chart()->addSeries(series);
    chartView->chart()->legend()->hide();
    chartView->setRenderHint(QPainter::Antialiasing);
}

cv::Mat HistogramEqualization::equalizeGrayImage(const cv::Mat &image)
{
    cv::Mat equalizedImage(image.size(), image.type());

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

    for (int i = 0; i < image.rows; i++)
    {
        for (int j = 0; j < image.cols; j++)
        {
            equalizedImage.at<uchar>(i, j) = grayscaleSet[image.at<uchar>(i, j)];
        }
    }

    return equalizedImage;
}
