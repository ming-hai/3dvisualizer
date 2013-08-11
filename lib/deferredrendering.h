/*
  Q Light Controller Plus
  deferredrendering.h

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

#include <GL/glew.h>

#include "depthrendertexture.h"
#include "fborendertexture.h"
#include "scenenode.h"
#include "viewport.h"
#include "shader.h"

/**
*	This object is used to render a big screen sized quad with the deferred rendering shader applied on it.
*/
class DeferredRendering
{
public:
    DeferredRendering(int width, int height, ViewPort *view);

    void    setSize(int w, int h);

	// Methods
    FBORenderTexture *getFBO();
    void    setLightMatrices(Matrix4f worldToLightViewMatrix,
                             Matrix4f lightViewToProjectionMatrix,
                             Matrix4f worldToCameraViewMatrix);
    void    startRenderToFBO();
    void    stopRenderToFBO();
    void    startRenderToShadowMap();
    void    stopRenderToShadowMap();
    void	showTexture(unsigned int i, float fSizeX = 400, float fSizeY = 400, float x = 0, float y = 0) const;
    void	showShadowMap(float fSizeX = 400, float fSizeY = 400, float x = 0, float y = 0) const;
	void	render();

private:
	// Variables
    Shader 		        m_shader; // Deferred rendering shader
    FBORenderTexture    m_fboRenderTexture; // The FBO render texture that contains diffuse, normals and positions
    DepthRenderTexture  m_shadowMap; // The FBO that renders the depth into the shadow map
    ViewPort*           m_view; // Reference to the main viewport
    SceneNode*          m_screenQuad; // Screen quad model reference

	unsigned int		m_width; // width
	unsigned int		m_height; // height

    Matrix4f            m_worldToLightViewMatrix; // Matrix that takes a vector from World Space into Light View Space
    Matrix4f            m_lightViewToProjectionMatrix; // Matrix that takes a vector from View Space into Projection Space (Clip Space)
    Matrix4f            m_worldToCameraViewMatrix; // Matrix that takes a vector from World Space into Camera View Space

    GLuint				m_diffuseLoc; // Diffuse texture handle for the shader
    GLuint				m_positionLoc; // Position texture handle for the shader
    GLuint				m_normalsLoc; // Normals texture handle for the shader
    GLuint				m_shadowMapLoc; // Shadow Map texture handle for the shader

    GLuint              m_worldToLightViewMatrixLoc; // Shader ID for the specified matrix
    GLuint              m_lightViewToProjectionMatrixLoc; // Shader ID for the specified matrix
    GLuint              m_worldToCameraViewMatrixLoc; // Shader ID for the specified matrix
};

