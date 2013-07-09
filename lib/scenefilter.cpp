/*
  Q Light Controller Plus
  scenefilter.cpp

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

#include "scenefilter.h"
#include "sceneviewer.h"

SceneFilter::SceneFilter(SceneViewer *scv)
{
    m_scv = scv;
    m_shader = NULL;
    m_deferredShader = NULL;
    m_shadowShader = NULL;
}

SceneFilter::~SceneFilter(void)
{
}

void SceneFilter::setShader(ShaderData *shader)
{
    m_shader = shader;
}

void SceneFilter::addFBTexture(Uniforms target, QString textureName)
{
    FrameBufferTextureBinder *tex = new FrameBufferTextureBinder(target, textureName, m_scv);
    m_textures.append(tex);
}

bool SceneFilter::bind(enum DrawingPass pass)
{
    if((pass == DrawingPassTransparent) != isTransparent)
		return false;

	//get correct Shader
    ShaderData* curShader = NULL;
	switch (pass)
	{
    case DrawingPassDeferred:
        curShader = m_deferredShader;
		break;
	case DrawingPassShadow:
        curShader = m_shadowShader;
		break;
	default:
        curShader = m_shader;
		break;
	}

    if(curShader == NULL)
		return false;
	else
        curShader->bind();

    Vector2f renderDim(m_scv->width(), m_scv->height());

    ShaderData::Uniform2fv(VecRenderSize, renderDim);

    foreach(FramebufferTexture *texture, m_textures)
        texture->bind();

    return true;
}


