TEMPLATE = app
QT += quickpath

SOURCES = main.cpp window.cpp
HEADERS = window.h

target.path = $$[QT_INSTALL_EXAMPLES]/hellonvpr
INSTALLS += target
