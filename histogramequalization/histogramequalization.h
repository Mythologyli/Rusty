#ifndef RUSTY_HISTOGRAMEQUALIZATION_H
#define RUSTY_HISTOGRAMEQUALIZATION_H

#include <QWidget>
#include <QChartView>
#include <opencv2/opencv.hpp>

namespace Ui
{
    class HistogramEqualization;
}

class HistogramEqualization : public QWidget
{
Q_OBJECT

public:
    explicit HistogramEqualization(QWidget *parent = nullptr);

    ~HistogramEqualization() override;

private:
    static void showGrayHistogram(const cv::Mat &image, QChartView *chartView);

    static cv::Mat equalizeGrayImage(const cv::Mat &image);

    Ui::HistogramEqualization *ui;
};

#endif // RUSTY_HISTOGRAMEQUALIZATION_H
