#ifndef RUSTY_MAINWINDOW_H
#define RUSTY_MAINWINDOW_H

#include <QMainWindow>
#include <QNetworkAccessManager>

#include "binarizeimage/binarizeimage.h"
#include "playvideo/playvideo.h"

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
};

#endif // RUSTY_MAINWINDOW_H
