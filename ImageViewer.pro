QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

include($$PWD/opencv470/opencv470.pri)

INCLUDEPATH += \
    imagelabel/

SOURCES += \
    main.cpp \
    imageviewer.cpp

HEADERS += \
    imageviewer.h \
    imagelabel/imagelabel.hpp

FORMS += \
    imageviewer.ui

TRANSLATIONS += \
    ImageViewer_zh_CN.ts
CONFIG += lrelease
CONFIG += embed_translations

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resource.qrc

RC_FILE += app.rc
