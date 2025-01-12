QT += quick core multimedia gui network

HEADERS += \
        imagehandler.h \
        webhandler.h

SOURCES += \
        main.cpp \
        imagehandler.cpp \
        webhandler.cpp

resources.files = main.qml 
resources.prefix = /$${TARGET}
RESOURCES += resources
CONFIG += qml objective_c++
# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

QMAKE_INFO_PLIST = /Users/andrew/QtCreations/ReceiptTracker/build/Qt_6_7_2_for_iOS-Debug/Info.plist



# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target


