#include "include/mainwindow.h"
#include "ui_mainwindow.h"
#include <QDesktopServices>
#include <QFile>
#include <QMessageBox>
#include <QNetworkReply>
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

    // Http server on localhost:8080 for handing responses/callbacks
    auto replyHandler = new QOAuthHttpServerReplyHandler(8080, this);

    spotify_.setReplyHandler(replyHandler);
    spotify_.setAuthorizationUrl(QUrl("https://accounts.spotify.com/authorize"));
    spotify_.setAccessTokenUrl(QUrl("https://accounts.spotify.com/api/token"));
    spotify_.setClientIdentifier(client_id_);
    spotify_.setClientIdentifierSharedKey(client_secret_);
    spotify_.setScope("user-read-private "
                      "user-top-read "
                      "playlist-read-private "
                      "playlist-modify-public "
                      "playlist-modify-private");

    connect(&spotify_, &QOAuth2AuthorizationCodeFlow::authorizeWithBrowser, &QDesktopServices::openUrl);

    connect(&spotify_, &QOAuth2AuthorizationCodeFlow::statusChanged, this, &MainWindow::auth_status_changed);

    connect(ui->login_button_, &QPushButton::clicked, this, &MainWindow::login);
}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::auth_status_changed(QAbstractOAuth::Status status)
{

    if (status == QAbstractOAuth::Status::Granted)
    {
        QUrl u("https://api.spotify.com/v1/me");

        auto reply = spotify_.get(u);

        connect(reply, &QNetworkReply::finished, [=]() {
            if (reply->error() != QNetworkReply::NoError)
            {
                // Probably add error message here
                return;
            }
            const auto data              = reply->readAll();
            nlohmann::json user_info_msg = nlohmann::json::parse(data.toStdString());
            QString::fromStdString(user_info_msg["id"]);

            ui->username_label_->setText(QString::fromStdString(user_info_msg["id"]));

            reply->deleteLater();
        });
    }
}
void MainWindow::login()
{
    spotify_.grant();
}
