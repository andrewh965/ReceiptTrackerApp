#include "imagehandler.h"
#include <QDebug>
#include <QTemporaryFile>
#include <QImageWriter>
#include <QDir>
#include <QDebug>
#include <QPixmap>
#include <QTextStream>
#include <QStringList>
#include <QImage>
#define HOME QDir::homePath()
#define garbageVals 2


void pwd(){
    QString doc = HOME+"/Documents";
    QDir dir(doc);
    QStringList fileNames = dir.entryList(QDir::Files);
    foreach(const QString &fileName, fileNames){
        qDebug()<<fileName;
    }
}
//Stdout: Store:SAFEWAY | Total:42.11

QString parseData(QString target, const QString Buff){
    QString dat;
    QString targetCpy = target.toUpper();
    QString BuffCpy   = Buff.toUpper();

    int i = BuffCpy.indexOf(targetCpy) + target.length() + garbageVals;
    while(Buff[i] != '|'){
        dat.append(Buff[i]);
        i++;
    }
    return dat;

}
/*************************************************************/

receipt::receipt(QString s, double t, QString d,int i): m_store(s),m_total(t),m_date(d),m_id(i){};

/*************************************************************/



receiptModel::receiptModel(QObject *parent):QAbstractListModel(parent){

}

void receiptModel::addReceipt(const QString& retrieved){
    beginResetModel();
    this->receiptList.clear();
    endResetModel();
    QJsonDocument jsonDoc = QJsonDocument::fromJson(retrieved.toUtf8());
     if(jsonDoc.isNull()) return;
    QJsonArray jsonArray = jsonDoc.array();
     beginInsertRows(QModelIndex(),rowCount(),rowCount()+jsonArray.size()-1);

    for(const QJsonValue &value:jsonArray){
        QJsonObject jsonObj = value.toObject();

        QString store = jsonObj["STORE"].toString();
        double  total = jsonObj["TOTAL"].toDouble();
        QString date  = jsonObj["DATE_VISITED"].toString();
        int id        = jsonObj["ID"].toInt();
        date.remove("T07:00:00.000Z");
        receiptList.append({store,total,date,id});

    }
    endInsertRows();
}


QHash<int,QByteArray> receiptModel::roleNames() const{
    QHash<int, QByteArray> roles;
    roles[storeRole] = "store";
    roles[totalRole] = "total";
    roles[dateRole]  = "date";
    roles[idRole]    = "ID";
    return roles;
}

QVariant receiptModel::data(const QModelIndex & index, int role) const{
    if (index.row() < 0 || index.row() >= receiptList.count())
        return QVariant();

    const receipt &retrieved = receiptList[index.row()];
    if (role == storeRole)
        return retrieved.store();
    else if (role == totalRole)
        return retrieved.total();
    else if(role == dateRole){
        return retrieved.date();
    }else if(role == idRole){
        return retrieved.id();
    }
    return QVariant();
}

double receiptModel::getTotal(){
    double total = 0;
    for(int i = 0; i<receiptList.size();i++){
        total += receiptList[i].total();
    }
    return total;
}



/*************************************************************/
/*************************************************************/


calendarModel::calendarModel(QObject *parent):QAbstractListModel(parent){}

void calendarModel::fillList(const QString& retrieved, int view){

    QString viewMode;
    if(view == 0){
        viewMode = "YEAR";
    }else if(view == 1){
        viewMode = "MONTH";
    }

    beginResetModel();
    this->calendarList.clear();
    endResetModel();
    QJsonDocument jsonDoc = QJsonDocument::fromJson(retrieved.toUtf8());
    if(jsonDoc.isNull()) return;
    QJsonArray jsonArray = jsonDoc.array();


    beginInsertRows(QModelIndex(),rowCount(),rowCount()+jsonArray.size()-1);
    for(const QJsonValue &value:jsonArray){
        QJsonObject jsonObj = value.toObject();
        calendarList.append(jsonObj[viewMode+"(DATE_VISITED)"].toInt());
    }
    endInsertRows();
}

QHash<int,QByteArray> calendarModel::roleNames() const{
    QHash<int, QByteArray> roles;
    roles[calendarRole] = "calendarVal";
    return roles;
}

QVariant calendarModel::data(const QModelIndex & index, int role) const{
    if (index.row() < 0 || index.row() >= calendarList.count())
        return QVariant();

    const int retrieved = calendarList[index.row()];
    if (role == calendarRole){
        return retrieved;
    }
    return QVariant();
}


/*************************************************************/
/*************************************************************/

ImageHandler::ImageHandler(QObject *parent): QObject(parent){
    QString doc = HOME+"/Documents";
    QDir dir(doc);
    QStringList fileNames = dir.entryList(QDir::Files);
    foreach(const QString &fileName, fileNames){
        dir.remove(fileName);
    }
    //printSql();

}

/*************************************************************/



/*************************************************************/
//TODO: Image popup/handler

void ImageHandler::setCalendarModel(calendarModel& ymodel){
     QString receipt_dir = json_dir + "/2/0/0/0";
     wh->setUrl(receipt_dir);
     if(wh->GET(this->Buff)==-1) qDebug()<<"Get failed";
     ymodel.fillList(Buff,0);
     return;
}

void ImageHandler::setCalendarModel(calendarModel* mmodel, int year){
    QString receipt_dir = json_dir+"/3/"+QString::number(year)+"/0/0";
    wh->setUrl(receipt_dir);
    if(wh->GET(this->Buff) == -1) qDebug()<<"Get failed";
    mmodel->fillList(Buff,1);
    return;
}



void ImageHandler::setReceiptModel(int year, int month, receiptModel* rmodel){
    QString receipt_dir = json_dir + "/0/"+QString::number(year)+"/"+ QString::number(month)+"/0";
    qDebug()<<receipt_dir;
    wh->setUrl(receipt_dir);
    if(wh->GET(this->Buff)==-1) qDebug()<<"Post failed";
    qDebug()<<Buff;
    rmodel->addReceipt(Buff);
    return;
}


void ImageHandler::submitForm(QString store, QString total){
    wh->setUrl(json_dir);
    if(store == NULL || total == NULL) qDebug()<< "inputs are null";
    qDebug()<<store<<" "<<total;
    if(wh->sendJson(store,total) == -1) qDebug()<<"Post failed";
    return;
}

void ImageHandler::printImage(int id){

    QString doc = HOME+"/Documents";

    int dig = 0;
    int tmp = id;
    while(tmp != 0){
        dig++;
        tmp/=10;
    }

    QString jpgNum = QString::number(id).rightJustified(5-dig,'0');
    QString fileName = doc+"/img_"+jpgNum+".jpg";
    wh->setUrl(mhtd_dir);
    if(wh->POST(fileName, this->Buff)==-1) qDebug()<<"Post failed";
    emit notifyUser(parseData("Store",Buff) , parseData("Total",Buff));

}

void ImageHandler::getPix(int receiptID){

    int jsonFormat = 4, jsonMeta = 35;
    if (receiptID<1) return;
    QString receipt_dir = json_dir + "/1/0/0/"+QString::number(receiptID);
    wh->setUrl(receipt_dir);
    if(wh->GET(this->Buff)==-1) qDebug()<<"Post failed";

    QString tmpFile = HOME+"/Documents/tmp.jpg";
    QFile tmpJPG(tmpFile);

    this->Buff.remove(this->Buff.length()-jsonFormat,jsonFormat);
    this->Buff.remove(0,jsonMeta);
    QByteArray hexString;
    QStringList decvals = this->Buff.split(',');
    for(int i = 0;i<decvals.length();i++){
        hexString.append(static_cast<char>(decvals[i].toInt()));
    }


    if(tmpJPG.open(QIODevice::WriteOnly|QIODevice::Text)){
        tmpJPG.write(hexString);
        tmpJPG.close();
    }else{
        qDebug()<<"Error: Could not write to file";
    }



    this->Buff.clear();
    return;
}

void ImageHandler::deleteTmp(){
    QString tmpFile = HOME+"/Documents/tmp.jpg";
    QFile tmpJPG(tmpFile);
    tmpJPG.remove();
}

QString ImageHandler::getTmp(){
     return QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation);
}

double ImageHandler::getTotal(receiptModel* rmodel){
    return rmodel->getTotal();
}
