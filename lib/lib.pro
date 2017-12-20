QT = core gui widgets
TEMPLATE=lib
CONFIG+=sharedlib
TARGET=syntax-highlighting

SOURCES = \
    highlighter.cxx \
    repository.cxx \
    tinyxml2.cpp

HEADERS = \
    highlighter.hh \
    repository.hh \
    rules.hh \
    tinyxml2.h
