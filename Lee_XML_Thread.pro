QMAKE_CXXFLAGS += -std=gnu++0x

TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
    Buzon.cpp \
    Semaforo.cpp \
    LeeXML.cpp

HEADERS += \
    Buzon.h \
    Semaforo.h \
    LeeXML.h

