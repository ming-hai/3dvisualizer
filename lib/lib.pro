
TEMPLATE = lib
LANGUAGE = C++
TARGET   = qlcplus3dvisualizer

CONFIG  += qt
CONFIG  += release
CONFIG  -= debug
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
    camera.h \
    objectseditor.h \
    scenenode.h \
    math_3d.h \
    gbuffer.h \
    pipeline.h \
    technique.h \
    lights_common.h \
    ds_dir_light_pass_tech.h \
    ds_point_light_pass_tech.h \
    ds_light_pass_tech.h \
    ds_geom_pass_tech.h \
    null_technique.h

SOURCES += visualizer3d.cpp \
    sceneviewer.cpp \
    camera.cpp \
    objectseditor.cpp \
    scenenode.cpp \
    math_3d.cpp \
    gbuffer.cpp \
    pipeline.cpp \
    technique.cpp \
    ds_dir_light_pass_tech.cpp \
    ds_point_light_pass_tech.cpp \
    ds_light_pass_tech.cpp \
    ds_geom_pass_tech.cpp \
    null_technique.cpp


FORMS += objectseditor.ui

#############################################################################
# Installation
#############################################################################

target.path = ../output
INSTALLS   += target

