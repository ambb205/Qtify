#include "include/spotify_result.h"
#include <QNetworkAccessManager>
#include <iostream>
Spotify_Result::Spotify_Result(nlohmann::json item_msg, QObject *parent) : QObject(parent)
{

    artist_        = QString::fromStdString(item_msg["artists"][0]["name"]);
    album_         = QString::fromStdString(item_msg["album"]["name"]);
    song_          = QString::fromStdString(item_msg["name"]);
    play_uri_      = QString::fromStdString(item_msg["uri"]);
    album_art_url_ = QString::fromStdString(item_msg["album"]["images"][2]["url"]);

    QNetworkAccessManager *nam = new QNetworkAccessManager;
    connect(nam, &QNetworkAccessManager::finished, this, &Spotify_Result::download_finished);
    const QUrl url = QUrl(album_art_url_);
    QNetworkRequest request(url);
    nam->get(request);
}

void Spotify_Result::paint(QPainter *painter, const QRect &rect, const QPalette &palette) const
{
    painter->drawPixmap(rect.x(), rect.y() + 5, 64, 64, *album_art_);
    painter->drawText(rect.x() + 76, rect.y() + 16, "Song:\t" + song_);
    painter->drawText(rect.x() + 76, rect.y() + 38, "Artist:\t" + artist_);
    painter->drawText(rect.x() + 76, rect.y() + 60, "Album:\t" + album_);
}

void Spotify_Result::download_finished(QNetworkReply *reply)
{

    album_art_ = new QPixmap;
    album_art_->loadFromData(reply->readAll());
    downloaded_ = true;
    emit downloaded(rect_);
}

QSize Spotify_Result::sizeHint() const
{
    return QSize(64, 74);
}
void Spotify_Result::set_rect(const QRect &rect)
{
    rect_ = rect;
}

QString Spotify_Result::get_play_uri()
{
    return play_uri_;
}
