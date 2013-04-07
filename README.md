3D Visualizer for QLC+

Environment setup
============

Download MinGW installer here:<br>
http://sourceforge.net/projects/mingw/files/Installer/mingw-get-inst/<br>
and install both MinGW and MSYS in the default path proposed by the installer.<br>

Get latest Qt prebuilt for MinGW32 here:<br>
http://releases.qt-project.org/qt4/source/<br>
and install it in C:\Qt

Make sure your environment variables are set as follows:

QMAKESPEC = win32-g++<br>
QTDIR = C:\Qt\4.8.4<br>
PATH = ....;C:\Qt\4.8.4\bin;C:\Qt\4.8.4;C:\MinGW\bin<br>

Download and install latest version of assimp library:<br>
http://sourceforge.net/projects/assimp/files/<br>
and install it in C:\Qt\Assimp

Download the latest version of GLEW library with the -mingw32-dev.tar.lzma suffix:<br>
http://lrn.no-ip.info/other/mingw/mingw32/glew/<br>
Extract it in your MinGW installation root folder<br>
Do the same with the package ending with -mingw32-dll-0.tar.lzma

Download the latest version of FREEGLUT library with the -mingw32-dev.tar.lzma suffix:<br>
http://lrn.no-ip.info/other/mingw/mingw32/freeglut/<br>
Extract it in your MinGW installation root folder<br>
Do the same with the package ending with -mingw32-dll-0.tar.lzma

Clone, compile and install
============
git clone git://github.com/mcallegari/3dvisualizer.git

cd 3dvisualizer<br>
qmake<br>
make 
make install
