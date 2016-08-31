TEMPLATE = lib
QT += gui-private quick
TARGET = QtQuickPath

load(qt_module)

DEFINES += QTQUICKPATH_BUILD_DLL

SOURCES += $$PWD/qnvpr.cpp \
           $$PWD/qnvprrendernode.cpp \
           $$PWD/qquickpathrendernode.cpp \
           $$PWD/qquickpathrendermaterial.cpp \
           $$PWD/qquickpathitem.cpp

HEADERS += $$PWD/qnvpr.h \
           $$PWD/qnvpr_p.h \
           $$PWD/qtquickpathglobal.h \
           $$PWD/qquickabstractpathrenderer_p.h \
           $$PWD/qnvprrendernode_p.h \
           $$PWD/qquickpathrendernode_p.h \
           $$PWD/qquickpathrendermaterial_p.h \
           $$PWD/qquickpathitem_p.h