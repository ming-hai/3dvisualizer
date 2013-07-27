/*
  Q Light Controller Plus
  viewport.h

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

#ifndef VIEWPORT_H
#define VIEWPORT_H

#include "math_3d.h"

class ViewPort
{
public:
    ViewPort();

    Matrix4f projectionMatrix();

    void setZ(float z);
    float getZ();

private:
    void setProjectionMatrix();

    float m_Xpos, m_Ypos, m_Zpos;
    Matrix4f viewProjectionMatrix;
};

#endif // VIEW_H
