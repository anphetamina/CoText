QT = websockets
QT += sql

TARGET = sslechoserver
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

SOURCES += \
    main.cpp \
    sslechoserver.cpp

HEADERS += \
    sslechoserver.h

EXAMPLE_FILES += sslechoclient.html

RESOURCES += securesocketclient.qrc

INSTALLS += target