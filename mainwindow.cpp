#include <QMessageBox>
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

    networkAccessManager->get(QNetworkRequest(QUrl("https://rusty.myth.cx/")));

    connect(ui->binarizeImageAction, &QAction::triggered,
            [&]()
            {

                binarizeImage = new BinarizeImage(this);
                binarizeImage->setWindowFlag(Qt::Window);
                binarizeImage->show();
            });
}

MainWindow::~MainWindow()
{
    delete networkAccessManager;

    delete binarizeImage;

    delete ui;
}
