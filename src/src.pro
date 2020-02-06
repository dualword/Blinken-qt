TEMPLATE = app
TARGET = blinken-qt
DEPENDPATH += .
INCLUDEPATH += .

CONFIG += qt widgets thread release
QT += svg
#QT += phonon 

QMAKE_CXXFLAGS += -std=c++11

HEADERS += blinken.h \
           blinkengame.h \
           button.h \
           counter.h \
           highscoredialog.h \
           number.h \
           soundsplayer.h
           
SOURCES += main.cpp \
		   blinken.cpp \
           blinkengame.cpp \
           button.cpp \
           counter.cpp \
           highscoredialog.cpp \
           number.cpp \
           soundsplayer.cpp

RESOURCES   = app.qrc
OBJECTS_DIR = .build/obj
MOC_DIR     = .build/moc
RCC_DIR     = .build/rcc
UI_DIR      = .build/ui
