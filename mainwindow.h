#ifndef RUSTY_MAINWINDOW_H
#define RUSTY_MAINWINDOW_H

#include <QMainWindow>
#include <QNetworkAccessManager>

#include "binarizeimage/binarizeimage.h"

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
};

#endif // RUSTY_MAINWINDOW_H
