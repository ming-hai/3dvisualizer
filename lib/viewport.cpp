/*
  Q Light Controller Plus
  viewport.cpp

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

#include "viewport.h"
#include "math_3d.h"

#include <QDebug>

ViewPort::ViewPort()
{
    m_Xpos = 1.0;
    m_Ypos = -1.0;
    m_Zpos = -2.0;

    setProjectionMatrix();
}

void ViewPort::setZ(float z)
{
    m_Zpos = z;
    setProjectionMatrix();
}

float ViewPort::getZ()
{
    return m_Zpos;
}

void ViewPort::setProjectionMatrix()
{
    Matrix4f projection, translation, rotation;

    PersProjInfo info;
    info.FOV = 60.0f;
    info.Height = 1.0f;
    info.Width  = 1.0f;
    info.zNear = 0.1f;
    info.zFar = 100.0f;

    Vector3f fromCenter = Vector3f(m_Xpos, m_Ypos, m_Zpos);

    projection.InitPersProjTransform(info);
    //projection.InitOrthoTransform(-1.0f, 1.0f, -1.0f, 1.0f, -1.0f, 1.0f);

    translation.InitTranslationTransform(fromCenter.x,fromCenter.y,fromCenter.z);

    rotation.InitCameraTransform(-fromCenter, Vector3f(0.0f, 1.0f, 0.0f));

    viewProjectionMatrix = projection * rotation * translation;
}


Matrix4f ViewPort::projectionMatrix()
{
    return viewProjectionMatrix;
}
