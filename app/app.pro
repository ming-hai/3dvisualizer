#-------------------------------------------------
#
# Project created by QtCreator 2013-02-28T17:35:44
#
#-------------------------------------------------
CONFIG   += release
QT       += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = 3DVisualizer
TEMPLATE = app
INCLUDEPATH += ../lib
LIBS        += -L../lib -L../lib/release -lqlcplus3dvisualizer

win32 {
    ASSIMP_DIR = C:/Qt/Assimp
    LIBS       += -L$$ASSIMP_DIR/bin/x86 -lAssimp32
    LIBS       += -lglew32 -lglut
    INCLUDEPATH += $$ASSIMP_DIR/include
    #QMAKE_LFLAGS += -shared
} else {
    CONFIG += link_pkgconfig

    system(pkg-config --exists assimp)
    {
        PKGCONFIG += assimp
    }
    LIBS    += -lGLEW -lGLU -lglut
}

SOURCES += main.cpp

HEADERS  +=

target.path = ../output
INSTALLS   += target
