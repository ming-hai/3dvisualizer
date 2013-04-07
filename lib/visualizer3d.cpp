/*
  Q Light Controller
  visualizer3d.cpp

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

#include <QHBoxLayout>
#include <QSplitter>
#include <QTextEdit>

Visualizer3D::Visualizer3D(QWidget *parent)
    : QWidget(parent)
    , m_splitter(NULL)
{
    resize(640,480);
    new QHBoxLayout(this);
    layout()->setContentsMargins(0, 0, 0, 0);
    layout()->setSpacing(0);

    m_splitter = new QSplitter(Qt::Horizontal, this);

    layout()->addWidget(m_splitter);

    m_sceneView = new SceneViewer();
    m_editor = new ObjectsEditor(this, m_sceneView);

    // 3D Scene container + Viewer
    QWidget* scontainer = new QWidget(this);
    scontainer->setLayout(new QVBoxLayout);
    m_splitter->addWidget(scontainer);
    m_splitter->widget(0)->layout()->addWidget(m_sceneView);
    //m_splitter->widget(0)->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);

    // Object editor container + UI
    QWidget* econtainer = new QWidget(this);
    econtainer->setLayout(new QVBoxLayout);
    m_splitter->addWidget(econtainer);
    m_splitter->widget(1)->layout()->addWidget(m_editor);
    m_splitter->widget(1)->setMaximumWidth(500);
    //m_splitter->widget(1)->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Preferred);

    m_splitter->show();
}

Visualizer3D::~Visualizer3D()
{
    delete m_sceneView;
}
