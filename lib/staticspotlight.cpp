/*
  Q Light Controller Plus
  staticspotlight.cpp

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

#include <QDir>

#include "staticspotlight.h"

StaticSpotlight::StaticSpotlight(ViewPort *view, SceneNode *parent)
    : SceneNode(view, parent)
{
    m_emitLight = true;
    loadModel(QString(MODELS_FOLDER) + QDir::separator() + "spotlight.obj");


}

void StaticSpotlight::attachLight(Spotlight *light)
{
    m_light = light;
}
