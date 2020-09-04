#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "include/spotify_result.h"
#include <QListWidgetItem>
#include <QMainWindow>
#include <QOAuth2AuthorizationCodeFlow>
QT_BEGIN_NAMESPACE
namespace Ui
{
    class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QString client_id, QString client_secret, QWidget *parent = nullptr);
    ~MainWindow();
private slots:
    void auth_status_changed(QAbstractOAuth::Status status);
    void login();

    void on_search_line_edit_textChanged(const QString &arg1);

    void on_search_results_view_itemDoubleClicked(QListWidgetItem *item);

private:
    Ui::MainWindow *ui;
    QOAuth2AuthorizationCodeFlow spotify_;
    const QString client_id_     = "";
    const QString client_secret_ = "";
    std::vector<Spotify_Result *> results_;
};
#endif // MAINWINDOW_H
