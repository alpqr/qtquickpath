TEMPLATE = lib
QT += quick
TARGET = QtNVPR

load(qt_module)

DEFINES += QTNVPR_BUILD_DLL

SOURCES += $$PWD/qnvpr.cpp \
           $$PWD/qnvprrendernode.cpp \
           $$PWD/qpathrendernode.cpp \
           $$PWD/qquickpathitem.cpp

HEADERS += $$PWD/qnvpr.h \
           $$PWD/qnvpr_p.h \
           $$PWD/qtnvprglobal.h \
           $$PWD/qquickabstractpathrendernode_p.h \
           $$PWD/qnvprrendernode_p.h \
           $$PWD/qpathrendernode_p.h \
           $$PWD/qquickpathitem_p.h
