/*
  Q Light Controller Plus
  texturedata.cpp

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

#include <QList>
#include <QDebug>

#include "texture.h"
#include "sceneviewer.h"

Texture::Texture(SceneViewer* sv, ShaderData *shader, QObject *parent)
    : QObject(parent)
{
    textureId = GLUINT_MAX;
    m_sv = sv;
    m_shader = shader;
}

Texture::~Texture()
{
    if(textureId != GLUINT_MAX)
        glDeleteTextures( 1, &textureId );
}

void Texture::initData()
{
	// allocate a texture name
	glGenTextures( 1, &textureId );
    qDebug() << "Texture ID:" << textureId;
	// select our current texture
	glBindTexture( GL_TEXTURE_2D, textureId );
	// set the minification filter
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST );
	// set the magnification filter
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
}

bool Texture::loadMaterial(QColor color)
{
    initData();
    quint8 mat[4] = { (quint8)color.red(), (quint8)color.green(), (quint8)color.blue(), (quint8)color.alpha() };
    if (color.red() == 0 && color.green() == 0 && color.blue() == 0)
        mat[0] = mat[1] = mat[2] = 200;
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, (void *)mat);
    return true;
}

bool Texture::loadTexture(QString path)
{
    initData();
    return false;
}

void Texture::bind(void)
{
    //qDebug() << "Bind texture ID:" << textureId;
    glActiveTexture(GL_TEXTURE0 + m_sv->getTextureUnitCount());
    glBindTexture(GL_TEXTURE_2D, textureId);
    m_shader->Uniform1i(Target, m_sv->getTextureUnitCount());
    m_sv->increaseTextureUnitCount();
}

/*
TextureData* TextureData::FromDDS(char* source)
{
    foreach(TextureData *texture, Textures)
	{
        if(texture->Name == source)
            return texture;
	}

	TextureData* Texture = new TextureData();
	Texture->CreateData();
	DDSLoader::loadDds(source);
	strcpy(Texture->Name,source);
	//Textures.Add(textureData);

	return Texture;
}
*/

Texture* Texture::SetTarget(enum Uniforms target)
{
	Target = target;
	return this;
}
