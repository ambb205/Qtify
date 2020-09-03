#include "include/mainwindow.h"

#include <QApplication>
#include <QFile>
#include <QMessageBox>
#include <QTextStream>
#include <nlohmann/json.hpp>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QString client_id     = "";
    QString client_secret = "";

    QFile key_file(":resources/keys.json");
    if (!key_file.exists())
    {
        printf("Unable to open keys file, file not found\n");
    }
    else
    {
        key_file.open(QFile::ReadOnly | QFile::Text);

        QTextStream key_text_stream(&key_file);

        QString key_json_string;

        key_json_string = key_text_stream.readAll();

        nlohmann::json keys_json = nlohmann::json::parse(key_json_string.toStdString());
        client_id                = QString::fromStdString(keys_json["id"]);
        client_secret            = QString::fromStdString(keys_json["secret"]);
        key_file.close();
    }
    if (client_id == "" && client_secret == "")
    {
        QMessageBox::critical(NULL, "Qtify", "Error, no keys!\nCheck resources/keys.json file and fill in your own keys");
        return 0;
    }
    else
    {

        MainWindow w(client_id, client_secret);
        w.show();
        return a.exec();
    }
}
