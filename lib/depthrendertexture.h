/*
  Q Light Controller Plus
  depthrendertexture.h

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

#ifndef DEPTHRENDERTEXTURE_H
#define DEPTHRENDERTEXTURE_H

#include "GL/glew.h"

/**
*	With this class we wrap the ability of OpenGL to render into a Depth Texture. We need this to create the shadow map.
*/
class DepthRenderTexture
{
public:
	// Ctors/Dtors
	DepthRenderTexture(int width, int height);
	~DepthRenderTexture();

    void    setSize(int w, int h);

	// Methods
    void    update();
	void	start();
	void	stop();
    void	showTexture(float fSizeX = 400, float fSizeY = 400, float x = 0, float y = 0) const;

	GLuint	getTexture() const { return m_depthTexture; }

private:

	// Variables
	GLuint			m_fbo; // The FBO ID
    GLuint			m_depthBufferRT; // Depth buffer handle
	GLuint			m_depthTexture; // Texture

	unsigned int	m_width; // FBO width
	unsigned int	m_height; // FBO height
};

#endif
