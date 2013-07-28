/*
  Q Light Controller Plus
  depthrendertexture.cpp

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

#include <QDebug>

#include "depthrendertexture.h"

/**
*	Create the FBO render texture initializing all the stuff that we need
*/
DepthRenderTexture::DepthRenderTexture(int _dWidth, int _dHeight)
{	
	// Save extensions
	m_width  = _dWidth;
	m_height = _dHeight;

    // Generate the OGL resources for what we need
    glGenFramebuffersEXT(1, &m_fbo);
    glGenRenderbuffersEXT(1, &m_depthBufferRT);
    glGenTextures(1, &m_depthTexture);

    update();
}

void DepthRenderTexture::update()
{
    // Bind the FBO so that the next operations will be bound to it
    glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, m_fbo);

    // Bind the depth buffer
    glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, m_depthBufferRT);
    glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, GL_DEPTH_COMPONENT24, m_width, m_height);
    glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_RENDERBUFFER_EXT, m_depthBufferRT);

    // Generate and bind the OGL texture for diffuse

    glBindTexture(GL_TEXTURE_2D, m_depthTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, m_width, m_height, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_INT, 0);
    // Attach the texture to the FBO
    glFramebufferTexture2D(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_depthTexture, 0);

    // Check if all worked fine and unbind the FBO
    GLenum status = glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT);
    if( status != GL_FRAMEBUFFER_COMPLETE_EXT)
        qDebug() << "Can't initialize an FBO render texture. FBO initialization failed.";

    glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
}

/**
*	Destructor
*/
DepthRenderTexture::~DepthRenderTexture(){
	glDeleteFramebuffersEXT(1, &m_fbo);
    glDeleteRenderbuffersEXT(1, &m_depthBufferRT);
    glDeleteTextures(1, &m_depthTexture);
}

void DepthRenderTexture::setSize(int w, int h)
{
    m_width = w;
    m_height = h;
    update();
}

/**
*	Start rendering to the texture
*	Both color and depth buffers are cleared.
*/
void DepthRenderTexture::start()
{
	// Bind our FBO and set the viewport to the proper size
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, m_fbo);
	glPushAttrib(GL_VIEWPORT_BIT);
	glViewport(0,0,m_width, m_height);

	// Clear the render target
	glClear( GL_DEPTH_BUFFER_BIT );

    // Specify that we need no colours
    glDrawBuffer(GL_NONE);
}

/**
*	Stop rendering to this texture.
*/
void DepthRenderTexture::stop()
{
	// Stop acquiring and unbind the FBO
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
	glPopAttrib();
}

/**
*	Show the texture to screen. It is just for debug purposes.
*/
void DepthRenderTexture::showTexture(float fSizeX, float fSizeY, float x, float y) const
{
	//Projection setup
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
    glOrtho(0,m_width,0,m_height,0.1f,2);

	//Model setup
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();

	glActiveTextureARB(GL_TEXTURE0_ARB);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, m_depthTexture);

	// Render the quad
	glLoadIdentity();
    glTranslatef(x,-y,-1.0);

    glColor3f(1,1,1);
    glBegin(GL_QUADS);
    glTexCoord2f( 0, 1 );
    glVertex3f(    0.0f,  (float) m_height, 0.0f);
    glTexCoord2f( 0, 0 );
    glVertex3f(    0.0f,   m_height-fSizeY, 0.0f);
    glTexCoord2f( 1, 0 );
    glVertex3f(   fSizeX,  m_height-fSizeY, 0.0f);
    glTexCoord2f( 1, 1 );
    glVertex3f(   fSizeX, (float) m_height, 0.0f);
    glEnd();

	glBindTexture(GL_TEXTURE_2D, 0);

	//Reset to the matrices	
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
}

