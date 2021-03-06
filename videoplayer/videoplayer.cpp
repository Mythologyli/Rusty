#include <QTimer>
#include <QFileDialog>

#include "../utility/utility.h"
#include "videoplayer.h"
#include "ui_videoplayer.h"

VideoPlayer::VideoPlayer(QWidget *parent) :
        QWidget(parent),
        ui(new Ui::VideoPlayer),
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
                                            cv::Point(20, frame.rows - 20),
                                            cv::FONT_HERSHEY_SIMPLEX,
                                            2.0,
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
                    timer->start(static_cast<int>(1000.0 / fps));
                    ui->lineEdit->setText("ASCII only. FPS: " + QString::number((int) fps));
                }
            });
}

VideoPlayer::~VideoPlayer()
{
    if (videoCapture != nullptr)
    {
        videoCapture->release();
        delete videoCapture;
    }

    delete ui;
}
