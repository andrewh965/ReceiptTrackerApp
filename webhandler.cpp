#include "webhandler.h"
#include <QHttpMultiPart>


WebHandler::WebHandler() {

}

WebHandler::~WebHandler(){}



int WebHandler::GET(QString& buff){
    QNetworkReply *reply = manager.get(QNetworkRequest(serverUrl));
    QEventLoop loop;
    QObject::connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();

    if(reply->error()==QNetworkReply::NoError){
        QString Response = reply->readAll();
        buff = Response;
        //qDebug()<<"Server: "<<buff;
    }else{
        qDebug()<<"Error: "<<reply->errorString();
        return -1;
    }
    return 0;
}

int WebHandler::sendJson(QString store, QString total){
    QJsonObject json;
    json["Store"] = store;
    json["Total"] = total;
    QJsonDocument doc(json);
    QByteArray jsonData = doc.toJson();
    QNetworkRequest request(serverUrl);
    request.setHeader(QNetworkRequest::ContentTypeHeader,"application/json");
    QNetworkReply *reply = manager.post(request, jsonData);
    QEventLoop loop;
    QObject::connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();

    if(reply->error()!=QNetworkReply::NoError){
        qDebug()<<"Error: "<<reply->errorString();
        return -1;
    }
    return 0;
}

void WebHandler::setUrl(const QString serverUrl)
{
    this->serverUrl.setUrl(serverUrl);
}

int WebHandler::POST(const QString& filePath,QString& buff){
    //retrieve file from phone
    QFile *file = new QFile(filePath);
    if(!file->open(QIODevice::ReadOnly)){
        delete file;
        return -1;
    }
    //multipart required for sending files
    QHttpMultiPart *multiPart = new QHttpMultiPart(QHttpMultiPart::FormDataType);
    QHttpPart filePart;

    //set headers and body
    filePart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data; name=\"file\"; filename=\"" + QFileInfo(filePath).fileName() + "\""));
    filePart.setBodyDevice(file);
    file->setParent(multiPart); // ensure file is deleted with multiPart
    multiPart->append(filePart);
    QNetworkRequest rq(serverUrl);

    QNetworkReply *reply = manager.post(rq,multiPart);

    //wait for response and notify user
    QEventLoop loop;
    QObject::connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();

    if(reply->error()==QNetworkReply::NoError){
        buff = reply->readAll();
    }else{
        qDebug()<<"Error: "<<reply->errorString();
        return -1;
    }
    reply->deleteLater();
    return 0;

}
