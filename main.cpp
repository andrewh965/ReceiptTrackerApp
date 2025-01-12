#include <QGuiApplication>
#include <QQmlContext>
#include <QQmlApplicationEngine>
#include "imagehandler.h"
int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    const QUrl url(QStringLiteral("qrc:/ReceiptTracker/main.qml"));
    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreated,
        &app,
        [url](QObject *obj, const QUrl &objUrl) {
            if (!obj && url == objUrl)
                QCoreApplication::exit(-1);
        },
        Qt::QueuedConnection);

    ImageHandler imageHandler;
    receiptModel receiptModel;
    calendarModel    yearModel;
    calendarModel   monthModel;

    imageHandler.setCalendarModel(yearModel);


    engine.rootContext()->setContextProperty("imageHandler", &imageHandler);
    engine.rootContext()->setContextProperty("receiptModel", &receiptModel);
    engine.rootContext()->setContextProperty("yearModel", &yearModel);
    engine.rootContext()->setContextProperty("monthModel", &monthModel);


    engine.load(url);

    return app.exec();
}

