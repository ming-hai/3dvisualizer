/*
  Q Light Controller Plus
  deferredrendering.cpp

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

#include "fborendertexture.h"
#include "deferredrendering.h"

/**
*	Create the deferred rendering object. I have hardcoded the shader's name here.
*/
DeferredRendering::DeferredRendering(int _dWidth, int _dHeight, ViewPort *view)
    : m_shader("shaders/deferredRendering.vert", "shaders/deferredRendering.frag")
    , m_fboRenderTexture(_dWidth, _dHeight)
    , m_shadowMap(1024, 1024)
    , m_view(view)
	, m_width(_dWidth)
	, m_height(_dHeight)
{	
	// Get the handles from the shader
    m_diffuseLoc = glGetUniformLocation(m_shader.m_shaderProg, "tDiffuse");
    m_positionLoc = glGetUniformLocation(m_shader.m_shaderProg, "tPosition");
    m_normalsLoc = glGetUniformLocation(m_shader.m_shaderProg, "tNormals");
    m_shadowMapLoc = glGetUniformLocationARB(m_shader.m_shaderProg,"tShadowMap");

    m_worldToLightViewMatrixLoc = glGetUniformLocationARB(m_shader.m_shaderProg,"worldToLightViewMatrix");
    m_lightViewToProjectionMatrixLoc = glGetUniformLocationARB(m_shader.m_shaderProg,"lightViewToProjectionMatrix");
    m_worldToCameraViewMatrixLoc = glGetUniformLocationARB(m_shader.m_shaderProg,"worldToCameraViewMatrix");

    m_screenQuad = new SceneNode(view);
    QString screenQuad =
            "mtllib floor.mtl\n"
            "o Plane\n"
            "v 1.000000 -1.000000 0.000000\n"
            "v -1.000000 -1.000000 0.000000\n"
            "v 1.000000 1.000000 0.000000\n"
            "v -1.000000 1.000000 0.000000\n"
            "vt 0.000000 1.000000\n"
            "vt 1.000000 1.000000\n"
            "vt 1.000000 0.000000\n"
            "vt 0.000000 0.000000\n"
            "vn 0.000000 0.000000 1.000000\n"
            "usemtl\n"
            "s off\n"
            "f 2/1/1 1/2/1 3/3/1\n"
            "f 2/1/1 3/3/1 4/4/1\n";
    m_screenQuad->loadModelFromBuffer(screenQuad);
    m_screenQuad->setShader(&m_shader);
}

void DeferredRendering::setSize(int w, int h)
{
    m_width = w;
    m_height = h;
    m_fboRenderTexture.setSize(w, h);
    m_shadowMap.setSize(w, h);
}

/**
*	Acquire the light matrices
*/
void DeferredRendering::setLightMatrices(float worldToLightViewMatrix[16], float lightViewToProjectionMatrix[16], float worldToCameraViewMatrix[16])
{
    memcpy(m_worldToLightViewMatrix, worldToLightViewMatrix, sizeof(float) * 16);
    memcpy(m_lightViewToProjectionMatrix, lightViewToProjectionMatrix, sizeof(float) * 16);
    memcpy(m_worldToCameraViewMatrix, worldToCameraViewMatrix, sizeof(float) * 16);
}

/**
*	Start rendering to the FBO
*/
void DeferredRendering::startRenderToFBO()
{
    m_fboRenderTexture.start();
}

/**
*	Stop rendering to the FBO
*/
void DeferredRendering::stopRenderToFBO()
{
    m_fboRenderTexture.stop();
}

/**
*	Render the debug texture
*/
void DeferredRendering::showTexture(unsigned int i, float fSizeX, float fSizeY, float x, float y) const
{
    m_fboRenderTexture.showTexture(i, fSizeX, fSizeY, x, y);
}

/**
*	Render the depth into the shadow map
*/
void DeferredRendering::startRenderToShadowMap()
{
    m_shadowMap.start();
}

/**
*	Stop to render the depth into the shadow map
*/
void DeferredRendering::stopRenderToShadowMap()
{
    m_shadowMap.stop();
}

/**
*	Render the debug texture for the shadow map
*/
void DeferredRendering::showShadowMap( float fSizeX, float fSizeY, float x, float y) const
{
    m_shadowMap.showTexture(fSizeX, fSizeY, x, y);
}

/**
*	Render the big quad with the deferred rendering shader on it
*/
void DeferredRendering::render()
{
    glUseProgram(m_shader.m_shaderProg);

	glActiveTextureARB(GL_TEXTURE0_ARB);
	glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, m_fboRenderTexture.getDiffuseTexture());
    glUniform1iARB ( m_diffuseLoc, 0 );
	
	glActiveTextureARB(GL_TEXTURE1_ARB);
	glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, m_fboRenderTexture.getPositionTexture());
    glUniform1iARB ( m_positionLoc, 1 );
	
	glActiveTextureARB(GL_TEXTURE2_ARB);
	glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, m_fboRenderTexture.getNormalsTexture());
    glUniform1iARB ( m_normalsLoc, 2 );

    glActiveTextureARB(GL_TEXTURE3_ARB);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, m_shadowMap.getTexture());
    glUniform1iARB ( m_shadowMapLoc, 3 );

    glUniformMatrix4fv ( m_worldToCameraViewMatrixLoc, 1, GL_FALSE, m_worldToCameraViewMatrix );
    glUniformMatrix4fv ( m_lightViewToProjectionMatrixLoc, 1, GL_FALSE, m_lightViewToProjectionMatrix );
    glUniformMatrix4fv ( m_worldToLightViewMatrixLoc, 1, GL_FALSE, m_worldToLightViewMatrix );


	// Render the quad
    m_screenQuad->render(false);

	// Reset OpenGL state
	glActiveTextureARB(GL_TEXTURE0_ARB);
	glDisable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);

	glActiveTextureARB(GL_TEXTURE1_ARB);
	glDisable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);

	glActiveTextureARB(GL_TEXTURE2_ARB);
	glDisable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);

    glActiveTextureARB(GL_TEXTURE3_ARB);
    glDisable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);

    glUseProgram(0);
}
