QT += core network sql
QT -= gui

CONFIG += c++11

TARGET = CMServer2
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += \
    database/basesqlcontroller.cpp \
    database/dbaccountcontroller.cpp \
    database/dbconnectedsetting.cpp \
    database/dbconnection.cpp \
    database/dbmessagecontroller.cpp \
    callentry.cpp \
    clientinstence.cpp \
    cmserver.cpp \
    main.cpp \
    entity/account.cpp \
    entity/messageinformation.cpp \
    crypto/diffhelmanprotocol.cpp \
    log/consoledebugloger.cpp \
    setting/cmserversetting.cpp

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

FORMS +=

DISTFILES +=

HEADERS += \
    database/basesqlcontroller.h \
    database/dbaccountcontroller.h \
    database/dbconnectedsetting.h \
    database/dbconnection.h \
    database/dbmessagecontroller.h \
    callentry.h \
    clientinstence.h \
    cmserver.h \
    protocol.h \
    entity/account.h \
    entity/messageinformation.h \
    crypto/diffhelmanprotocol.h \
    log/consoledebugloger.h \
    log/icmloger.h \
    setting/cmserversetting.h
