/*
  Q Light Controller Plus
  main.cpp

  Copyright (C) Massimo Callegari

  This program is free software; you can redistribute it and/or
  modify it under the terms of the GNU General Public License
  Version 2 as published by the Free Software Foundation.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details. The license is
  in the file "COPYING".

  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*/

#include "visualizer3d.h"

#include <QtGui>
#include <QApplication>

/**
 * Suppresses debug messages
 */
void messageHandler(QtMsgType type, const char* msg)
{
//#if defined(WIN32) || defined(__APPLE__)
//        if (QLCArgs::dbgBox != NULL)
//            QLCArgs::dbgBox->addText(msg);
//#else
        fprintf(stderr, "%d: %s\n", type, msg);
        fflush(stderr);
//#endif
}

int main(int argc, char *argv[])
{
    /* Handle debug messages */
    qInstallMsgHandler(messageHandler);

    QApplication app(argc, argv);
    Visualizer3D vis;
    vis.showMaximized();

    return app.exec();
}
