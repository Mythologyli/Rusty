#ifndef RUSTY_MAINWINDOW_H
#define RUSTY_MAINWINDOW_H

#include <QMainWindow>
#include <QNetworkAccessManager>

#include "imagebinarization/imagebinarization.h"
#include "videoplayer/videoplayer.h"
#include "histogramequalization/histogramequalization.h"
#include "imagesegmentation/imagesegmentation.h"
#include "imagemorphology/imagemorphology.h"
#include "imagerestoration/imagerestoration.h"
#include "reconstruction/reconstruction.h"

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

    ImageBinarization *imageBinarization;
    VideoPlayer *videoPlayer;
    HistogramEqualization *histogramEqualization;
    ImageSegmentation *imageSegmentation;
    ImageMorphology *imageMorphology;
    ImageRestoration *imageRestoration;
    Reconstruction *reconstruction;
};

#endif // RUSTY_MAINWINDOW_H
