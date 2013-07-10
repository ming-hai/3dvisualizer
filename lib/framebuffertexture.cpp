/*
  Q Light Controller Plus
  framebuffertexture.cpp

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

#include "sceneviewer.h"
#include "framebuffertexture.h"

float Anisotropic = 16;

FramebufferTexture::FramebufferTexture(SceneViewer *sv)
{
    textureId = GLUINT_MAX;

    m_sv = sv;
}

FramebufferTexture::~FramebufferTexture(void)
{
    if(textureId != GLUINT_MAX)
        glDeleteTextures( 1, &textureId );
}

void FramebufferTexture::initData()
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

void FramebufferTexture::bind(void)
{
    qDebug() << Q_FUNC_INFO << "textureID:" << textureId;
	if(ShaderData::HasUniform(Target))
	{
        glActiveTexture(GL_TEXTURE0 + m_sv->getTextureUnitCount());
		glBindTexture(GL_TEXTURE_2D, textureId);
        ShaderData::Uniform1i(Target, m_sv->getTextureUnitCount());
        m_sv->increaseTextureUnitCount();
	}
}

FramebufferTexture* FramebufferTexture::SetTarget(enum Uniforms target)
{
	Target = target;
	return this;
}
