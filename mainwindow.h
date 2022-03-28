#ifndef RUSTY_MAINWINDOW_H
#define RUSTY_MAINWINDOW_H

#include <QMainWindow>
#include <QNetworkAccessManager>

#include "binarizeimage/binarizeimage.h"
#include "playvideo/playvideo.h"
#include "histogramequalization/histogramequalization.h"

namespace Ui
{
    class MainWindow;
}

class MainWindow : public QMainWindow
{
Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

    ~MainWindow() override;

private:
    Ui::MainWindow *ui;

    QNetworkAccessManager *networkAccessManager;

    BinarizeImage *binarizeImage;
    PlayVideo *playVideo;
    HistogramEqualization *histogramEqualization;
};

#endif // RUSTY_MAINWINDOW_H
