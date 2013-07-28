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
	// Ctors/Dtors
    DeferredRendering(int width, int height, ViewPort *view);

    void    setSize(int w, int h);

	// Methods
    void    setLightMatrices(float worldToLightViewMatrix[16], float lightViewToProjectionMatrix[16], float worldToCameraViewMatrix[16]);
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
    FBORenderTexture    m_fboRenderTexture; // A pointer to the FBO render texture that contains diffuse, normals and positions
    DepthRenderTexture  m_shadowMap; // A pointer to the FBO that renders the depth into the shadow map
    ViewPort*           m_view;
    SceneNode*          m_screenQuad;

	unsigned int		m_width; // width
	unsigned int		m_height; // height

    float               m_worldToLightViewMatrix[16]; // Matrix that takes a vector from World Space into Light View Space
    float               m_lightViewToProjectionMatrix[16]; // Matrix that takes a vector from View Space into Projection Space (Clip Space)
    float               m_worldToCameraViewMatrix[16]; // Matrix that takes a vector from World Space into Camera View Space

    GLuint				m_diffuseLoc; // Diffuse texture handle for the shader
    GLuint				m_positionLoc; // Position texture handle for the shader
    GLuint				m_normalsLoc; // Normals texture handle for the shader
    GLuint				m_shadowMapLoc; // Shadow Map texture handle for the shader

    GLuint              m_worldToLightViewMatrixLoc; // Shader ID for the specified matrix
    GLuint              m_lightViewToProjectionMatrixLoc; // Shader ID for the specified matrix
    GLuint              m_worldToCameraViewMatrixLoc; // Shader ID for the specified matrix
};

