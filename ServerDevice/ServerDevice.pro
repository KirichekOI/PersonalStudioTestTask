TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        cmdhandler.cpp \
        cmdparser.cpp \
        main.cpp \
        mediator.cpp \
        multimeteremulator.cpp \
        serverdevice.cpp \
        serverdevicelogger.cpp

HEADERS += \
    Commands.h \
    cmdhandler.h \
    cmdparser.h \
    mediator.h \
    multimeterconstants.h \
    multimeteremulator.h \
    serverdevice.h \
    serverdevicelogger.h



unix:!macx: LIBS += -lpthread
