#ifndef RUSTY_PLAYVIDEO_H
#define RUSTY_PLAYVIDEO_H

#include <QWidget>
#include <opencv2/opencv.hpp>

namespace Ui
{
    class PlayVideo;
}

class PlayVideo : public QWidget
{
Q_OBJECT

public:
    explicit PlayVideo(QWidget *parent = nullptr);

    ~PlayVideo() override;

private:
    Ui::PlayVideo *ui;

    cv::VideoCapture *videoCapture;
    QTimer *timer;
};

#endif // RUSTY_PLAYVIDEO_H
