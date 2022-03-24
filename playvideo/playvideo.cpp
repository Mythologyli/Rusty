#include <QTimer>
#include <QFileDialog>

#include "../utility/utility.h"
#include "playvideo.h"
#include "ui_playvideo.h"

PlayVideo::PlayVideo(QWidget *parent) :
        QWidget(parent),
        ui(new Ui::PlayVideo),
        videoCapture(nullptr),
        timer(nullptr)
{
    ui->setupUi(this);

    connect(ui->openVideoButton, &QPushButton::clicked,
            [&]()
            {
                delete videoCapture;

                if (timer == nullptr)
                {
                    timer = new QTimer(this);
                    connect(timer, &QTimer::timeout,
                            [&]()
                            {
                                cv::Mat frame;
                                videoCapture->read(frame);

                                if (frame.empty())
                                {
                                    timer->stop();
                                    return;
                                }

                                cv::putText(frame,
                                            ui->lineEdit->text().toStdString(),
                                            cv::Point(10, frame.rows - 10),
                                            cv::FONT_HERSHEY_SIMPLEX,
                                            1.0,
                                            CV_RGB(0, 255, 0),
                                            2);

                                utility::showCvMatInLabel(frame, ui->videoLabel);
                            });
                }
                else
                {
                    timer->stop();
                }

                videoCapture = utility::openVideoFile(this);
                if (videoCapture->isOpened())
                {
                    double fps = videoCapture->get(cv::CAP_PROP_FPS);
                    timer->start((int) (1000.0 / fps));
                    ui->lineEdit->setText("Watermark, ASCII only. FPS: " + QString::number((int) fps));
                }
            });
}

PlayVideo::~PlayVideo()
{
    if (videoCapture != nullptr)
    {
        videoCapture->release();
        delete videoCapture;
    }

    delete ui;
}
