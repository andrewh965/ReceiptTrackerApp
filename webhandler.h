#ifndef WEBHANDLER_H
#define WEBHANDLER_H
#include <QtCore>
#include <QObject>
#include <QtNetwork>

class WebHandler
{
public:
    WebHandler();
    ~WebHandler();
    int GET(QString& buff);
    int POST(const QString& filePath, QString& buff);
    int sendJson(QString store, QString total);
    void setUrl(const QString serverUrl);
private:
    QNetworkAccessManager manager;
    QUrl serverUrl;
};

#endif // WEBHANDLER_H
