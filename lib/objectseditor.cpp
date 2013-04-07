/*
  Q Light Controller Plus
  objectseditor.cpp

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

#include <QTreeWidgetItem>
#include <QMessageBox>
#include <QFileDialog>

#include "objectseditor.h"

#define KObjectsColumnName      0
#define KObjectsColumnID        1

ObjectsEditor::ObjectsEditor(QWidget *parent, SceneViewer *sv)
  : QWidget(parent)
  , m_scViewer(sv)
  , m_selectedObject(NULL)
{
    Q_ASSERT(m_scViewer != NULL);
    setupUi(this);

    connect(m_objectsTree, SIGNAL(itemClicked(QTreeWidgetItem*,int)),
            this, SLOT(slotObjectSelected(QTreeWidgetItem*, int)));
}

ObjectsEditor::~ObjectsEditor()
{

}

void ObjectsEditor::slotAddModel()
{
    QFileDialog dialog(this);
    dialog.setWindowTitle(tr("Open a 3D Model"));
    dialog.setFileMode(QFileDialog::ExistingFiles);
    dialog.setAcceptMode(QFileDialog::AcceptOpen);

    //QString modelsDir = QDir::toNativeSeperators(QDir::homePath());
    QString filter = tr("All 3D Models (*.3ds *.obj)");

    dialog.setFilter(filter);

    /* Execute the dialog */
    if (dialog.exec() != QDialog::Accepted)
        return;

    QStringListIterator it(dialog.selectedFiles());
    while (it.hasNext() == true)
    {
        QString path = it.next();
        quint32 newID = m_scViewer->addNode(path);
        if (newID == SceneNode::invalidId())
        {
            QMessageBox::warning(this, tr("Unsupported 3D model"), tr("This 3D model cannot be loaded. Sorry."));
        }
        else
        {
            QFileInfo fi(path);
            QTreeWidgetItem *item = new QTreeWidgetItem(m_objectsTree);
            item->setText(KObjectsColumnName,  fi.baseName());
            item->setText(KObjectsColumnID,  QString("%1").arg(newID));
        }
    }
}

void ObjectsEditor::slotDeleteModel()
{
    if (m_objectsTree->selectedItems().count() > 0)
    {
        QTreeWidgetItem *item = m_objectsTree->selectedItems().first();
        quint32 id = item->text(KObjectsColumnID).toUInt();
        m_scViewer->deleteNode(id);
        m_scViewer->update();
    }
}

void ObjectsEditor::slotObjectSelected(QTreeWidgetItem *item, int)
{
    quint32 id = item->text(KObjectsColumnID).toUInt();
    SceneNode *node = m_scViewer->node(id);
    if (node != NULL)
    {
        m_selectedObject = node->getModel();
        /*
        m_xposSpin->setValue(m_selectedObject->x());
        m_yposSpin->setValue(m_selectedObject->y());
        m_zposSpin->setValue(m_selectedObject->z());
        */
    }
}

void ObjectsEditor::slotXpositionChanged(double val)
{
    Q_UNUSED(val)
    /*
    if (m_selectedObject != NULL)
        m_selectedObject->setX(val);
    */
    m_scViewer->update();
}

void ObjectsEditor::slotYpositionChanged(double val)
{
    Q_UNUSED(val)
    /*
    if (m_selectedObject != NULL)
        m_selectedObject->setY(val);
    */
    m_scViewer->update();
}

void ObjectsEditor::slotZpositionChanged(double val)
{
    Q_UNUSED(val)
    /*
    if (m_selectedObject != NULL)
        m_selectedObject->setZ(val);
    */
    m_scViewer->update();
}


