/*
  Q Light Controller Plus
  objectseditor.h

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

#ifndef OBJECTSEDITOR_H
#define OBJECTSEDITOR_H

#include <QWidget>
#include "ui_objectseditor.h"
#include "sceneviewer.h"

class ObjectsEditor : public QWidget, public Ui_ObjectsEditor
{
    Q_OBJECT
public:
    ObjectsEditor(QWidget *parent = 0, SceneViewer *sv = 0);
    ~ObjectsEditor();

private:
    SceneViewer *m_scViewer;
    SceneNode *m_selectedNode;

signals:

public slots:
    void slotAddModel();
    void slotAddSpotlight();
    void slotDeleteModel();
    void slotObjectSelected(QTreeWidgetItem *item, int);

    void slotXpositionChanged(double val);
    void slotYpositionChanged(double val);
    void slotZpositionChanged(double val);

    void slotXrotationChanged(double val);
    void slotYrotationChanged(double val);
    void slotZrotationChanged(double val);
    
};

#endif // OBJECTSEDITOR_H
