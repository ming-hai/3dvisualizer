
TEMPLATE = lib
LANGUAGE = C++
TARGET   = qlcplus3dvisualizer

CONFIG  += qt
CONFIG  += release
QT      += core xml gui opengl

# Assimp doesn't have a pkg-config prebuilt package for Windows
# so just go directly to the sdk folder - crap

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
#############################################################################
# Sources
#############################################################################

HEADERS += visualizer3d.h \
    sceneviewer.h \
    objectseditor.h \
    scenenode.h \
    glrenderer.h

SOURCES += visualizer3d.cpp \
    sceneviewer.cpp \
    objectseditor.cpp \
    scenenode.cpp \
    glrenderer.cpp

FORMS += objectseditor.ui

#############################################################################
# Installation
#############################################################################

target.path = ../output
INSTALLS   += target

