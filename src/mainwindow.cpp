#include "include/mainwindow.h"
#include "ui_mainwindow.h"
#include <QFile>
#include <QMessageBox>
#include <QOAuthHttpServerReplyHandler>
#include <QPalette>
#include <QPixmap>
#include <QTextStream>
#include <iostream>
#include <nlohmann/json.hpp>
MainWindow::MainWindow(QString client_id, QString client_secret, QWidget *parent) :
    QMainWindow(parent), ui(new Ui::MainWindow), client_id_(client_id), client_secret_(client_secret)
{
    ui->setupUi(this);
    QFile f(":qdarkstyle/style.qss");
    if (!f.exists())
    {
        printf("Unable to set stylesheet, file not found\n");
    }
    else
    {
        f.open(QFile::ReadOnly | QFile::Text);
        QTextStream ts(&f);
        qApp->setStyleSheet(ts.readAll());
        f.close();
    }

    //    auto replyHandler = new QOAuthHttpServerReplyHandler(8080, this);

    //    spotify_.setReplyHandler(replyHandler);
    //    spotify_.setAuthorizationUrl(QUrl("https://accounts.spotify.com/authorize"));
    //    spotify_.setAccessTokenUrl(QUrl("https://accounts.spotify.com/api/token"));
    //    spotify_.setClientIdentifier(client_id_);
    //    spotify_.setClientIdentifierSharedKey(client_secret_);
    //    spotify_.setScope("user-read-private "
    //                      "user-top-read "
    //                      "playlist-read-private "
    //                      "playlist-modify-public "
    //                      "playlist-modify-private");
}

MainWindow::~MainWindow()
{
    delete ui;
}
