#ifndef SPOTIFY_RESULT_H
#define SPOTIFY_RESULT_H

#include <QLabel>
#include <QNetworkReply>
#include <QObject>
#include <QPainter>
#include <nlohmann/json.hpp>
class Spotify_Result : public QObject
{
    Q_OBJECT
public:
    Spotify_Result() = default;
    Spotify_Result(nlohmann::json item_msg, QObject *parent = nullptr);
    ~Spotify_Result() = default;

    Spotify_Result(const Spotify_Result &) = default;

    void paint(QPainter *painter, const QRect &rect, const QPalette &palette) const;
    bool downloaded_ = false;
    QSize sizeHint() const;
    void set_rect(const QRect &rect);
    QString get_play_uri();
signals:
    void downloaded(const QRect &rect);
private slots:

    void download_finished(QNetworkReply *reply);

private:
    QString artist_        = "";
    QString album_         = "";
    QString song_          = "";
    QString album_art_url_ = "";
    QPixmap *album_art_;
    QRect rect_;
    QString play_uri_ = "";
};
Q_DECLARE_METATYPE(Spotify_Result *);
#endif // SPOTIFY_RESULT_H
