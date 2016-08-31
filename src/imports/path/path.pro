CXX_MODULE = qml
TARGET  = qmlpathplugin
TARGETPATH = QtQuick/PathItem
IMPORT_VERSION = 2.0

QT = quickpath-private qml-private core-private

SOURCES += plugin.cpp

load(qml_plugin)
