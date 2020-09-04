#include "include/mainwindow.h"
#include "include/spotify_delegate.h"
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
                      "playlist-modify-private "
                      "user-modify-playback-state");

    connect(&spotify_, &QOAuth2AuthorizationCodeFlow::authorizeWithBrowser, &QDesktopServices::openUrl);

    connect(&spotify_, &QOAuth2AuthorizationCodeFlow::statusChanged, this, &MainWindow::auth_status_changed);

    connect(ui->login_button_, &QPushButton::clicked, this, &MainWindow::login);

    ui->search_line_edit->setEnabled(false);
    ui->search_results_view->setEnabled(false);
    ui->search_results_view->setItemDelegate(new Spotify_Delegate);
}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::auth_status_changed(QAbstractOAuth::Status status)
{

    if (status == QAbstractOAuth::Status::Granted)
    {
        QString token = spotify_.token();
        std::cout << token.toStdString() << std::endl;
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
            ui->search_line_edit->setEnabled(true);
            ui->search_results_view->setEnabled(true);
            reply->deleteLater();
        });
    }
}
void MainWindow::login()
{
    spotify_.grant();
}

void MainWindow::on_search_line_edit_textChanged(const QString &arg1)
{

    for (int i = 0; i < results_.size(); i++) { delete results_[i]; }
    results_.clear();
    ui->search_results_view->clear();
    auto search = arg1;
    search.replace(" ", "%20");
    QString call = "https://api.spotify.com/v1/search?q=" + search + "&type=track&limit=10";

    QUrl u(call);

    auto reply = spotify_.get(u);

    connect(reply, &QNetworkReply::finished, [=]() {
        if (reply->error() != QNetworkReply::NoError)
        {
            // error message?
            return;
        }

        const auto data = reply->readAll();
        nlohmann::json json_msg;
        json_msg = nlohmann::json::parse(data.toStdString());
        for (int i = 0; i < json_msg["tracks"]["items"].size(); i++)
        {
            results_.push_back(new Spotify_Result(json_msg["tracks"]["items"][i]));
            QListWidgetItem *item = new QListWidgetItem;
            QString call          = "https://api.spotify.com/v1/search?q=" + search + "&type=track&limit=10";

            QUrl u(call);

            auto reply = spotify_.get(u);

            connect(reply, &QNetworkReply::finished, [=]() {
                if (reply->error() != QNetworkReply::NoError)
                {
                    // error message?
                    return;
                }

                const auto data = reply->readAll();
                nlohmann::json json_msg;
                json_msg = nlohmann::json::parse(data.toStdString());
                for (int i = 0; i < json_msg["tracks"]["items"].size(); i++)
                {
                    results_.push_back(new Spotify_Result(json_msg["tracks"]["items"][i]));
                    QListWidgetItem *item = new QListWidgetItem;

                    item->setData(0, QVariant::fromValue(results_.back()));
                    ui->search_results_view->addItem(item);

                    connect(results_.back(), &Spotify_Result::downloaded, ui->search_results_view,
                            QOverload<const QRect &>::of(&QListWidget::update));
                }

                reply->deleteLater();
            });
            item->setData(0, QVariant::fromValue(results_.back()));
            ui->search_results_view->addItem(item);

            connect(results_.back(), &Spotify_Result::downloaded, ui->search_results_view,
                    QOverload<const QRect &>::of(&QListWidget::update));
        }

        reply->deleteLater();
    });
}

void MainWindow::on_search_results_view_itemDoubleClicked(QListWidgetItem *item)
{
    if (item->data(0).canConvert<Spotify_Result *>())
    {
        Spotify_Result *result = qvariant_cast<Spotify_Result *>(item->data(0));
        QString play_uri       = result->get_play_uri();
        play_uri.replace(":", "%3A");
        QString call = "https://api.spotify.com/v1/me/player/queue?uri=" + play_uri;
        std::cout << call.toStdString() << std::endl;
        QUrl u(call);

        auto reply = spotify_.post(u);

        connect(reply, &QNetworkReply::finished, [=]() {
            if (reply->error() != QNetworkReply::NoError)
            {
                // error message? it
                std::cout << reply->errorString().toStdString() << std::endl;
                return;
            }

            reply->deleteLater();
        });
        call = "https://api.spotify.com/v1/me/player/next";

        QUrl u_next(call);

        reply = spotify_.post(u_next);

        connect(reply, &QNetworkReply::finished, [=]() {
            if (reply->error() != QNetworkReply::NoError)
            {
                // error message? it
                std::cout << reply->errorString().toStdString() << std::endl;
                return;
            }

            reply->deleteLater();
        });
    }
}
