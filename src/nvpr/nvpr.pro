TEMPLATE = lib
QT += quick
TARGET = QtNVPR

load(qt_module)

DEFINES += QTNVPR_BUILD_DLL

SOURCES += $$PWD/qnvpr.cpp

HEADERS += $$PWD/qnvpr.h \
           $$PWD/qnvpr_p.h \
           $$PWD/qtnvprglobal.h
