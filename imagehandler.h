
#ifndef IMAGEHANDLER_H
#define IMAGEHANDLER_H

#include <QObject>
#include <QImage>
#include <QStandardPaths>
#include <QQmlContext>
#include <QFileInfo>
#include "webhandler.h"


//const QString mhtd_dir =  "http://10.0.0.136:3000/upload";
//const QString json_dir =  "http://10.0.0.136:3000/receipt";
const QString mhtd_dir = "http://192.168.0.21:3000/upload";
const QString json_dir = "http://192.168.0.21:3000/receipt";
/**********************************************************/
class receipt{ //container for data

public:
    receipt(QString s, double p, QString d, int i);
    QString store() const {return m_store;};
    double total() const {return m_total;};
    QString date() const {return m_date;};
    int id() const{return m_id;};

private:
    QString m_store;
    double m_total;
    QString m_date;
    int m_id;
};

/**********************************************************/


/**********************************************************/
class receiptModel : public QAbstractListModel{
    Q_OBJECT

public:

    enum receiptRoles{
        storeRole = Qt::UserRole+1,
        totalRole,
        dateRole,
        idRole
    };
    receiptModel(QObject* parent = nullptr);
    void addReceipt(const QString& retrieved);
    QVariant data(const QModelIndex& index, int role) const override;
    int rowCount(const QModelIndex& parent = QModelIndex()) const override{
        Q_UNUSED(parent);
        return receiptList.count();
    };
    QHash<int,QByteArray> roleNames() const override;
    double getTotal();

private:
    QList <receipt> receiptList;

};
/**********************************************************/
/**********************************************************/
class calendarModel : public QAbstractListModel{
    Q_OBJECT

public:
    enum calendarRoles{
        calendarRole = Qt::UserRole+1
    };

    calendarModel(QObject* parent = nullptr);
    void fillList(const QString& retrieved, int view);
    QVariant data(const QModelIndex& index, int role) const override;
    int rowCount(const QModelIndex& parent = QModelIndex()) const override{
        Q_UNUSED(parent);
        return calendarList.count();
    };
    QHash<int,QByteArray> roleNames() const override;

private:
    QList <int> calendarList;
};
/**********************************************************/
/**********************************************************/

class ImageHandler : public QObject
{
    Q_OBJECT
public:
    explicit ImageHandler(QObject *parent = nullptr);

public slots:
    void printImage(int id);
    void submitForm(QString store, QString total);
    void setCalendarModel(calendarModel& ymodel);
    void setCalendarModel(calendarModel* mmodel, int year);
    void setReceiptModel(int year, int month, receiptModel* rmodel);
    void getPix(int receiptID);
    QString getTmp();
    void deleteTmp();
    double getTotal(receiptModel* rmodel);

signals:
    void imageCaptured(const QString &previewUrl);
    void notifyUser(QString store, QString total);
private:
    //WebHandler *jsH = new WebHandler(json_dir);
    WebHandler *wh = new WebHandler();
    QString Buff;
    QList<QObject *> receiptList;
};

#endif // IMAGEHANDLER_H
