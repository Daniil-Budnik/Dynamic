QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

CONFIG += c++11

RC_FILE += myapp.rc
SOURCES += \
    main.cpp \
    dynamicwindow.cpp \
    qcustomplot.cpp

HEADERS += \
    dynamicwindow.h \
    qcustomplot.h

FORMS += \
    dynamicwindow.ui

qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    IMG.qrc
