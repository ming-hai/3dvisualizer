/*
  Q Light Controller Plus
  viewport.cpp

  Copyright (C) Phillip Schulz-Theißen, Massimo Callegari

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
#include "shaderdata.h"

#include <QDebug>

ViewPort* curView;

ViewPort::ViewPort()
{
    m_Xpos = 1.0;
    m_Ypos = -1.0;
    m_Zpos = -2.0;

    setProjectionMatrix();
}

void ViewPort::bind()
{
    curView = this;
}

void ViewPort::insertViewProjectionMatrix()
{
    ShaderData::UniformMatrix4fv(MatViewProjection, curView->viewProjectionMatrix);
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

//    projection = Matrix4f(
//        1.0f,0.0f,0.0f,0.0f,
//        0.0f,1.0f,0.0f,0.0f,
//        0.0f,0.0f,-1.00200200f,-1.0f,
//        0.0f,0.0f,-0.200200200f,0.0f);

    Vector3f fromCenter = Vector3f(m_Xpos, m_Ypos, m_Zpos);

    projection.InitPersProjTransform(info);

    translation.InitTranslationTransform(fromCenter.x,fromCenter.y,fromCenter.z);

    rotation.InitCameraTransform(-fromCenter,Vector3f(0.0f,1.0f,0.0f));

    viewProjectionMatrix = projection * rotation * translation;

    /*
    qDebug() << Q_FUNC_INFO << "GENERATED MATRIX";
    qDebug() << viewProjectionMatrix.m_matrix[0][0] << viewProjectionMatrix.m_matrix[0][1] << viewProjectionMatrix.m_matrix[0][2] << viewProjectionMatrix.m_matrix[0][3];
    qDebug() << viewProjectionMatrix.m_matrix[1][0] << viewProjectionMatrix.m_matrix[1][1] << viewProjectionMatrix.m_matrix[1][2] << viewProjectionMatrix.m_matrix[1][3];
    qDebug() << viewProjectionMatrix.m_matrix[2][0] << viewProjectionMatrix.m_matrix[2][1] << viewProjectionMatrix.m_matrix[2][2] << viewProjectionMatrix.m_matrix[2][3];
    qDebug() << viewProjectionMatrix.m_matrix[3][0] << viewProjectionMatrix.m_matrix[3][1] << viewProjectionMatrix.m_matrix[3][2] << viewProjectionMatrix.m_matrix[3][3];
    */
}
