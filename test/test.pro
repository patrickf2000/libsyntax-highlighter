QT += core widgets gui
TEMPLATE=app
CONFIG+= c+11
TARGET=testHighlighter
LIBS+=-L../lib -lsyntax-highlighting
INCLUDEPATH+=../lib

SOURCES = \
    main.cxx \
    testwindow.cxx

FORMS += \
    testwindow.ui

HEADERS += \
    testwindow.hh
