TEMPLATE = lib
QT += gui-private quick
TARGET = QtNVPR

load(qt_module)

DEFINES += QTNVPR_BUILD_DLL

SOURCES += $$PWD/qnvpr.cpp \
           $$PWD/qnvprrendernode.cpp \
           $$PWD/qquickpathrendernode.cpp \
           $$PWD/qquickpathitem.cpp

HEADERS += $$PWD/qnvpr.h \
           $$PWD/qnvpr_p.h \
           $$PWD/qtnvprglobal.h \
           $$PWD/qquickabstractpathrenderer_p.h \
           $$PWD/qnvprrendernode_p.h \
           $$PWD/qquickpathrendernode_p.h \
           $$PWD/qquickpathitem_p.h
