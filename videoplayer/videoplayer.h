#ifndef RUSTY_VIDEOPLAYER_H
#define RUSTY_VIDEOPLAYER_H

#include <QWidget>
#include <opencv2/opencv.hpp>

namespace Ui
{
    class VideoPlayer;
}

class VideoPlayer : public QWidget
{
Q_OBJECT

public:
    explicit VideoPlayer(QWidget *parent = nullptr);

    ~VideoPlayer() override;

private:
    Ui::VideoPlayer *ui;

    cv::VideoCapture *videoCapture;
    QTimer *timer;
};

#endif // RUSTY_VIDEOPLAYER_H
