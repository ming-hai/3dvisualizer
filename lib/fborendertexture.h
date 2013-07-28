/*
  Q Light Controller Plus
  fborendertexture.h

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

#ifndef FBORENDERTEXTURE_H
#define FBORENDERTEXTURE_H

#include <GL/glew.h>

/**
*	A Frame Buffer Object is used by OpenGL to render into a texture. Specifically this implementation assumes that the
*	rendered model will provide diffuse, position and normal at the same time in a MRT fashion
*/
class FBORenderTexture
{
public:
	// Ctors/Dtors
	FBORenderTexture(int width, int height);
	~FBORenderTexture();

	// Methods
    void    setSize(int w, int h);
    void    update();
	void	start();
	void	stop();
	void	showTexture(unsigned int i, float fSizeX = 400, float fSizeY = 400, float x = 0, float y = 0) const;

	GLuint	getDiffuseTexture() const { return m_diffuseTexture; }
	GLuint	getPositionTexture() const { return m_positionTexture; }
	GLuint	getNormalsTexture() const { return m_normalsTexture; }

private:

	// Variables
	GLuint			m_fbo; // The FBO ID
	GLuint			m_diffuseRT; // The diffuse render target
	unsigned int	m_diffuseTexture; // The OpenGL texture for the diffuse render target
	GLuint			m_positionRT; // The position render target
	unsigned int	m_positionTexture; // The OpenGL texture for the position render target
	GLuint			m_normalsRT; // The normals render target
	unsigned int	m_normalsTexture; // The OpenGL texture for the normals render target
	GLuint			m_depthBuffer; // Depth buffer handle

	unsigned int	m_width; // FBO width
	unsigned int	m_height; // FBO height
};

#endif
