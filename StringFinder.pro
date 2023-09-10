QT       += core gui

greaterThan(QT_MAJOR_VERSION, 5): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    MyObjects/myparserrunnable.cpp \
    MyObjects/myqcombobox.cpp \
    MyObjects/myqtreewidget.cpp \
    MyObjects/myqtreewidgetitem.cpp \
    dialogchoosepath.cpp \
    filehelper.cpp \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    MyObjects/myparserrunnable.h \
    MyObjects/myqcombobox.h \
    MyObjects/myqtreewidget.h \
    MyObjects/myqtreewidgetitem.h \
    dialogchoosepath.h \
    filehelper.h \
    mainwindow.h

FORMS += \
    dialogchoosepath.ui \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    res/Icon.jpg
