/*
  Q Light Controller Plus
  texture.cpp

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

#include <QList>
#include <QDebug>

#include "texture.h"
#include "sceneviewer.h"

Texture::Texture(GLenum TextureTarget, QObject *parent)
    : QObject(parent)
{
    textureId = GLUINT_MAX;
    m_textureTarget = TextureTarget;
}

Texture::~Texture()
{
    if(textureId != GLUINT_MAX)
        glDeleteTextures( 1, &textureId );
}

void Texture::init()
{
	// allocate a texture name
	glGenTextures( 1, &textureId );
    qDebug() << "Texture ID:" << textureId;
	// select our current texture
    glBindTexture( m_textureTarget, textureId );
	// set the minification filter
    glTexParameterf( m_textureTarget, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST );
	// set the magnification filter
    glTexParameterf( m_textureTarget, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
}

bool Texture::loadMaterial(QColor color)
{
    init();
    quint8 mat[4] = { (quint8)color.red(), (quint8)color.green(), (quint8)color.blue(), (quint8)color.alpha() };
    if (color.red() == 0 && color.green() == 0 && color.blue() == 0)
        mat[0] = mat[1] = mat[2] = 200;
    glTexImage2D(m_textureTarget, 0, GL_RGBA, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, (void *)mat);
    return true;
}

bool Texture::loadTexture(QString path)
{
    init();
    return false;
}

void Texture::bind(GLenum TextureUnit)
{
    qDebug() << "Bind texture ID:" << textureId;
    glActiveTexture(TextureUnit);
    glBindTexture(m_textureTarget, textureId);
}

