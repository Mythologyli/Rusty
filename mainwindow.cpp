#include <QMessageBox>
#include <QHostInfo>
#include <QNetworkReply>

#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
        QMainWindow(parent),
        ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setWindowIcon(QIcon(":/resource/resource/icon.png"));

    connect(ui->aboutAction, &QAction::triggered,
            [&]()
            {
                QMessageBox::about(this, "关于", "License:\n\nGNU Lesser General Public License Version 3");
            });

    networkAccessManager = new QNetworkAccessManager(this);

    connect(networkAccessManager, &QNetworkAccessManager::finished,
            [&](QNetworkReply *reply)
            {
                if (reply->error() == QNetworkReply::NoError)
                {
                    ui->textBrowser->setText(QString(reply->readAll()));
                }
                else
                {
                    QString text = "<html><h3>发生错误！无法显示作业详情，请检查网络连接</h3>";
                    text += "<h3>您也可以直接查看 <a href=\"https://rusty.myth.cx\">https://rusty.myth.cx</a></h3></html>";
                    ui->textBrowser->setText(text);
                }

                reply->deleteLater();
            });

    QNetworkRequest request(QUrl("https://rusty.myth.cx/"));
    request.setHeader(QNetworkRequest::UserAgentHeader, "Rusty in " + QHostInfo::localHostName());
    networkAccessManager->get(request);

    connect(ui->imageBinarizationAction, &QAction::triggered,
            [&]()
            {
                imageBinarization = new ImageBinarization(this);
                imageBinarization->setWindowFlag(Qt::Window);
                imageBinarization->setAttribute(Qt::WA_DeleteOnClose);
                imageBinarization->show();
            });

    connect(ui->videoPlayerAction, &QAction::triggered,
            [&]()
            {
                videoPlayer = new VideoPlayer(this);
                videoPlayer->setWindowFlag(Qt::Window);
                videoPlayer->setAttribute(Qt::WA_DeleteOnClose);
                videoPlayer->show();
            });

    connect(ui->histogramEqualizationAction, &QAction::triggered,
            [&]()
            {
                histogramEqualization = new HistogramEqualization(this);
                histogramEqualization->setWindowFlag(Qt::Window);
                histogramEqualization->setAttribute(Qt::WA_DeleteOnClose);
                histogramEqualization->show();
            });

    connect(ui->imageSegmentationAction, &QAction::triggered,
            [&]()
            {
                imageSegmentation = new ImageSegmentation(this);
                imageSegmentation->setWindowFlag(Qt::Window);
                imageSegmentation->setAttribute(Qt::WA_DeleteOnClose);
                imageSegmentation->show();
            });

    connect(ui->imageMorphologyAction, &QAction::triggered,
            [&]()
            {
                imageMorphology = new ImageMorphology(this);
                imageMorphology->setWindowFlag(Qt::Window);
                imageMorphology->setAttribute(Qt::WA_DeleteOnClose);
                imageMorphology->show();
            });
}

MainWindow::~MainWindow()
{
    delete ui;
}
