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

#include "texturedata.h"
//#include "DDSLoader.h"

int CurTexUnit;

//TextureData* TextureCache[4096];
//char* TextureNames[4096];
QList<TextureData*> Textures;
int TextureCachePosition;

float Anisotropic = 16;

TextureData::TextureData(void)
{
	//Textures = ListContainer();
    textureId = GLUINT_MAX;

	// Add to texture list
    Textures.append(this);
}

void TextureData::CreateData()
{
	// allocate a texture name
	glGenTextures( 1, &textureId );
	// select our current texture
	glBindTexture( GL_TEXTURE_2D, textureId );
	// set the minification filter
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST );
	// set the magnification filter
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
	// enable anisotropic filtering
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, Anisotropic);
}

TextureData::~TextureData(void)
{
    if(textureId != GLUINT_MAX)
		glDeleteTextures( 1, &textureId );

    Textures.removeOne(this);
}

void TextureData::Bind(void)
{
	if(ShaderData::HasUniform(Target))
	{
		glActiveTexture(GL_TEXTURE0 + CurTexUnit);
		glBindTexture(GL_TEXTURE_2D, textureId);
		ShaderData::Uniform1i(Target, CurTexUnit);
		CurTexUnit++;
	}
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

TextureData* TextureData::SetTarget(enum Uniforms target)
{
	Target = target;
	return this;
}
