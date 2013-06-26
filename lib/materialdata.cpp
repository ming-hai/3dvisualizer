/*
  Q Light Controller Plus
  materialdata.cpp

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

#include <QDir>

#include "materialdata.h"
#include "shaderdata.h"

MaterialData::MaterialData(QString shaderName)
{
    m_shader = ShaderData::FromPlainText(
                QString("shaders%1%2.vs").arg(QDir::separator()).arg(shaderName),
                QString("shaders%1%2.fs").arg(QDir::separator()).arg(shaderName));
    m_isTransparent = false;
}

bool MaterialData::addTexture(TextureData *tex)
{
    m_textures.append(tex);
    return true;
}

void MaterialData::setTransparent(bool enable)
{
    m_isTransparent = enable;
}

bool MaterialData::bind(enum DrawingPass pass)
{
    if((pass == DrawingPassTransparent) != m_isTransparent)
        return false;

    m_shader->Bind();

    //ShaderData::Uniform2fv(VecRenderSize,CurRenderDim);

    foreach(TextureData *tex, m_textures)
        tex->bind();

    return true;
}
