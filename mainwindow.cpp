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

    connect(ui->actionAbout, &QAction::triggered,
            [&]()
            {
                QMessageBox::about(this, "关于", "License:\n\nGNU Lesser General Public License Version 3");
            });

    networkAccessManager = new QNetworkAccessManager(this);

    connect(networkAccessManager, &QNetworkAccessManager::finished,
            [&](QNetworkReply *reply)
            {
                auto text = QString(reply->readAll());
                ui->textBrowser->setText(text);
                reply->deleteLater();
            });

    networkAccessManager->get(QNetworkRequest(QUrl("https://rusty.myth.cx/")));
}

MainWindow::~MainWindow()
{
    delete networkAccessManager;

    delete ui;
}
