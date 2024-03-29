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
  , m_selectedNode(NULL)
{
    Q_ASSERT(m_scViewer != NULL);
    setupUi(this);

    m_propsToolBox->setEnabled(false);

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
    QString filter = tr("All 3D Models (*.3ds *.obj *.dae *.blend *.x *.lwo *.lws *.ac *.ply)");

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

void ObjectsEditor::slotAddSpotlight()
{
    quint32 newID = m_scViewer->addSpotlight();
    if (newID == SceneNode::invalidId())
    {
        QMessageBox::warning(this, tr("Unsupported 3D model"), tr("This 3D model cannot be loaded. Sorry."));
    }
    else
    {
        QTreeWidgetItem *item = new QTreeWidgetItem(m_objectsTree);
        item->setText(KObjectsColumnName,  "Spotlight");
        item->setText(KObjectsColumnID,  QString("%1").arg(newID));
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
        int idx = m_objectsTree->indexOfTopLevelItem(item);
        m_objectsTree->takeTopLevelItem(idx);
        delete item;
    }
}

void ObjectsEditor::slotObjectSelected(QTreeWidgetItem *item, int)
{
    quint32 id = item->text(KObjectsColumnID).toUInt();
    m_selectedNode = m_scViewer->node(id);
    if (m_selectedNode != NULL)
    {
        m_xposSpin->setValue(m_selectedNode->Xposition());
        m_yposSpin->setValue(m_selectedNode->Yposition());
        m_zposSpin->setValue(m_selectedNode->Zposition());

        m_xrotationSpin->setValue(m_selectedNode->Xrotation());
        m_yrotationSpin->setValue(m_selectedNode->Yrotation());
        m_zrotationSpin->setValue(m_selectedNode->Zrotation());

        m_propsToolBox->setEnabled(true);
    }
}

void ObjectsEditor::slotXpositionChanged(double val)
{
    if (m_selectedNode != NULL)
        m_selectedNode->setXposition(val);

    m_scViewer->update();
}

void ObjectsEditor::slotYpositionChanged(double val)
{
    if (m_selectedNode != NULL)
        m_selectedNode->setYposition(val);

    m_scViewer->update();
}

void ObjectsEditor::slotZpositionChanged(double val)
{
    if (m_selectedNode != NULL)
        m_selectedNode->setZposition(val);

    m_scViewer->update();
}

void ObjectsEditor::slotXrotationChanged(double val)
{
    if (m_selectedNode != NULL)
        m_selectedNode->setXrotation(val);

    m_scViewer->update();
}

void ObjectsEditor::slotYrotationChanged(double val)
{
    if (m_selectedNode != NULL)
        m_selectedNode->setYrotation(val);

    m_scViewer->update();
}

void ObjectsEditor::slotZrotationChanged(double val)
{
    if (m_selectedNode != NULL)
        m_selectedNode->setZrotation(val);

    m_scViewer->update();
}


