3D Visualizer for QLC+

Environment setup
============

Download MinGW installer here:
http://sourceforge.net/projects/mingw/files/Installer/mingw-get-inst/
and install both MinGW and MSYS in the default path proposed by the installer.

Get latest Qt prebuilt for MinGW32 here:
http://releases.qt-project.org/qt4/source/
and install it in C:\Qt

Make sure your environment variables are set as follows:

QMAKESPEC = win32-g++ 
QTDIR = C:\Qt\4.8.4 
PATH = ....;C:\Qt\4.8.4\bin;C:\Qt\4.8.4;C:\MinGW\bin 

Download and install latest version of assimp library:
http://sourceforge.net/projects/assimp/files/
and install it in C:\Qt\Assimp

Download the latest version of GLEW library with the -mingw32-dev.tar.lzma suffix:
http://lrn.no-ip.info/other/mingw/mingw32/glew/
Extract it in your MinGW installation root folder
Do the same with the package ending with -mingw32-dll-0.tar.lzma

Download the latest version of FREEGLUT library with the -mingw32-dev.tar.lzma suffix:
http://lrn.no-ip.info/other/mingw/mingw32/freeglut/
Extract it in your MinGW installation root folder
Do the same with the package ending with -mingw32-dll-0.tar.lzma

Clone, compile and install
============
git clone git://github.com/mcallegari/3dvisualizer.git

cd 3dvisualizer 
qmake 
make 
make install