#-------------------------------------------------
#
# Project created by QtCreator 2013-02-28T17:35:44
#
#-------------------------------------------------
CONFIG   += release
CONFIG   -= debug
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

	3DVIS_OUTPUT_PATH = ../output
	# Qt Libraries
    qtlibs.path  = $$3DVIS_OUTPUT_PATH
    release:qtlibs.files = $$(QTDIR)/bin/QtCore4.dll \
                           $$(QTDIR)/bin/QtGui4.dll \
                           $$(QTDIR)/bin/QtXml4.dll \
                           $$(QTDIR)/bin/QtScript4.dll

    debug:qtlibs.files = $$(QTDIR)/bin/QtCored4.dll \
                         $$(QTDIR)/bin/QtGuid4.dll \
                         $$(QTDIR)/bin/QtXmld4.dll \
                         $$(QTDIR)/bin/QtScriptd4.dll
    INSTALLS += qtlibs

    # MinGW library
    mingw.path = $$3DVIS_OUTPUT_PATH
    exists($$(SystemDrive)/MinGW/bin/mingwm10.dll) {
        mingw.files += $$(SystemDrive)/MinGW/bin/mingwm10.dll
    }

    exists($$(QTDIR)/../MinGW/bin/mingwm10.dll) {
        mingw.files += $$(QTDIR)/../MinGW/bin/mingwm10.dll
    }

    # GCC 4.4.0
    exists($$(SystemDrive)/MinGW/bin/libgcc_s_dw2-1.dll) {
        mingw.files += $$(SystemDrive)/MinGW/bin/libgcc_s_dw2-1.dll
    }

    exists($$(QTDIR)/../MinGW/bin/libgcc_s_dw2-1.dll) {
        mingw.files += $$(QTDIR)/../MinGW/bin/libgcc_s_dw2-1.dll
    }
    INSTALLS += mingw
	
	# Assimp library
	assimp.path = $$3DVIS_OUTPUT_PATH
	exists($$ASSIMP_DIR/bin/x86/Assimp32.dll) {
		assimp.files = $$ASSIMP_DIR/bin/x86/Assimp32.dll
	}
	INSTALLS += assimp
	
	# OpenGL libs
	gllibs.path = $$3DVIS_OUTPUT_PATH
    exists($$(SystemDrive)/MinGW/bin/glew32.dll) {
        gllibs.files += $$(SystemDrive)/MinGW/bin/glew32.dll
    }
    exists($$(SystemDrive)/MinGW/bin/libglut-0.dll) {
        gllibs.files += $$(SystemDrive)/MinGW/bin/libglut-0.dll
    }
	INSTALLS += gllibs
	
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
