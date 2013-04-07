/*
  Q Light Controller Plus
  visualizer3d.h

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

#ifndef VISUALIZER3D_H
#define VISUALIZER3D_H

#include "objectseditor.h"
#include "sceneviewer.h"

class QSplitter;

class Visualizer3D : public QWidget
{
    Q_OBJECT
public:
    Visualizer3D(QWidget *parent = 0);
    ~Visualizer3D();

private:
    QSplitter* m_splitter; // vertical splitter (Scene3D + ObjectEditor)
    SceneViewer *m_sceneView;
    ObjectsEditor *m_editor;
};

#endif // VISUALIZER3D_H
