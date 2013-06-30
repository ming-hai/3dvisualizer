/*
  Q Light Controller Plus
  shaderdata.cpp

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

#include <QDebug>

#include "framebufferdata.h"
#include "sceneviewer.h"
//#include "Sun.h"
//#include "CubeMap.h"

/*********************************************************************
 * FrameBufferData class
 *********************************************************************/

FrameBufferData::FrameBufferData(void)
{
    FboId = GLUINT_MAX;
    DepthTextureId = GLUINT_MAX;
    TextureId = GLUINT_MAX;
    Clearcolor = Qt::black;
	DepthBufferFmt = GL_DEPTH_COMPONENT16;
	BufferFmt = GL_RGBA8;
    MultiSampling = false;
}

FrameBufferData::~FrameBufferData(void)
{
	glDeleteTextures(1, &TextureId);
	glDeleteTextures(1, &DepthTextureId);
	glDeleteFramebuffersEXT(1, &FboId);
}

void FrameBufferData::Bind(bool clear)
{
	if(FboReady)
	{
		// set rendering destination to FBO
		glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, FboId);
		glViewport(0, 0, SizeX, SizeY);

		// clear buffers
		if(clear)
		{
            glClearColor(Clearcolor.red(), Clearcolor.green(), Clearcolor.blue(), Clearcolor.alpha());
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		}
	}
}

void FrameBufferData::SetMultiSampling(bool value)
{
    MultiSampling = value;
    GLfloat param = MultiSampling ? GL_LINEAR : GL_NEAREST;

	if(UseColor)
	{
		glBindTexture(GL_TEXTURE_2D, TextureId);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, param);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, param);
	}

	glBindTexture(GL_TEXTURE_2D, DepthTextureId);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, param);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, param);
}

void FrameBufferData::Initialize()
{
    qDebug() << "FrameBufferData ENTER Initialize";

    GLfloat sampeling = MultiSampling ? GL_LINEAR : GL_NEAREST;

	if(UseColor)
	{
		// Create Color Tex
        if(TextureId == GLUINT_MAX)
			glGenTextures(1, &TextureId);
		glBindTexture(GL_TEXTURE_2D, TextureId);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, sampeling);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, sampeling);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexImage2D(GL_TEXTURE_2D, 0, BufferFmt, SizeX, SizeY, 0,
					GL_RGBA, GL_UNSIGNED_BYTE, 0);
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	// Create Depth Tex
    if(DepthTextureId == GLUINT_MAX)
		glGenTextures(1, &DepthTextureId);
	glBindTexture(GL_TEXTURE_2D, DepthTextureId);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, sampeling);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, sampeling);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexImage2D(GL_TEXTURE_2D, 0, DepthBufferFmt, SizeX, SizeY, 0,
		GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, 0);
	glBindTexture(GL_TEXTURE_2D, 0);

	// create a framebuffer object
    if(FboId == GLUINT_MAX)
		glGenFramebuffersEXT(1, &FboId);

	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, FboId);

	GLenum status = GL_FRAMEBUFFER_COMPLETE_EXT;
	if(UseColor)
	{
		// attach the texture to FBO color attachment point
		glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT,GL_TEXTURE_2D, TextureId, 0);
        status = glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT);
	}
	else
	{
		// Instruct openGL that we won't bind a color texture with the currently bound FBO
		glDrawBuffer(GL_NONE);
		glReadBuffer(GL_NONE);
	}

	// attach the depth exture to depth attachment point
	glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT,GL_TEXTURE_2D, DepthTextureId, 0);
	GLenum FBOstatus = glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT);

	// check FBO status
	if(status == GL_FRAMEBUFFER_COMPLETE_EXT && FBOstatus == GL_FRAMEBUFFER_COMPLETE_EXT)
		FboReady = true;
	else
		FboReady = false;

	// switch back to window-system-provided framebuffer
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);

	// check error
    qDebug() << "FrameBufferData Initialize";
}

/*********************************************************************
 * DefaultFrameBuffer class
 *********************************************************************/

DefaultFrameBuffer::DefaultFrameBuffer()
{
    m_width = 1024;
    m_height = 768;
}

void DefaultFrameBuffer::Bind(bool clear)
{
    // unbind FBO
    glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
    glViewport(0, 0, m_width, m_height);

    if(clear)
    {
        glClearColor(0.0, 0.0, 0.0, 1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }
    //glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, 0);
}

void DefaultFrameBuffer::setSize(int w, int h)
{
    m_width = w;
    m_height = h;
}

/*********************************************************************
 * BufferSet class
 *********************************************************************/

BufferSet::BufferSet()
{
	EnableSsao = false;
	EnableBloom = false;
    SizeX = 0;
    SizeY = 0;
}

void BufferSet::Initialize()
{
    NormalPass.SizeX = SizeX;
    NormalPass.SizeY = SizeY;
	NormalPass.DepthBufferFmt = GL_DEPTH_COMPONENT24;
	NormalPass.Initialize();

    DeferredLightmap.SizeX = SizeX;
    DeferredLightmap.SizeY = SizeY;
	DeferredLightmap.Initialize();

    ScenePass.SizeX = SizeX;
    ScenePass.SizeY = SizeY;
	ScenePass.Initialize();

    ReflectionPass.SizeX = SizeX;
    ReflectionPass.SizeY = SizeY;
	ReflectionPass.Initialize();

	if(EnableSsao)
	{
        Bloom.SizeX = SizeX/4;
        Bloom.SizeY = SizeY/4;
		Bloom.Initialize();

        BloomB.SizeX = SizeX/4;
        BloomB.SizeY = SizeY/4;
		BloomB.Initialize();
	}

	if(EnableSsao)
	{
        SsaoPrepare.SizeX = SizeX;
        SsaoPrepare.SizeY = SizeY;
		SsaoPrepare.BufferFmt = GL_RGBA16;
		SsaoPrepare.Initialize();

        SsaoPerform.SizeX = SizeX/4;
        SsaoPerform.SizeY = SizeY/4;
		SsaoPerform.Initialize();

        SsaoBlur.SizeX = SizeX/4;
        SsaoBlur.SizeY = SizeY/4;
        SsaoBlur.MultiSampling = true;
		SsaoBlur.Initialize();
	}

    OutBuffer = new DefaultFrameBuffer(); //(FrameBufferData*)&defaultFramebuffer;
}

BufferSet::~BufferSet()
{
}

static QString const BufferNames[] = {
	"NULL",
	"NormalColor",
	"NormalDepth",
	"DefferedLightmap",
	"MyShadowmap",
	"MyInnerShadowMap",
	"Scene",
	"Bloom",
	"BloomB",
	"SsaoPrepare",
	"SsaoPerform",
	"SsaoBlur",
	"CubemapSide1",
	"CubemapSide2",
	"CubemapSide3",
	"CubemapSide4",
	"CubemapSide5",
	"CubemapSide6",
	"DefferedReflections"
};

static int const BufferNameCount = sizeof(BufferNames)/sizeof(BufferNames[0]);

FbTextureBinder::FbTextureBinder(enum Uniforms target, QString texture, SceneViewer *sv)
{
    m_sv = sv;

    // Assign Uniform Target
    m_target = target;

    m_bufferSet = m_sv->getBufferSet();

	// Assign Texture Target
    m_texture = FrameBufferNull;
	for (int i = 0; i < BufferNameCount; i++)
	{
        if(BufferNames[i] == texture)
		{
            m_texture = (FrameBuffer)i;
			break;
		}
	}
}

void FbTextureBinder::bind()
{
    if(ShaderData::HasUniform(m_target))
	{
		GLuint textureId = -1;
        switch (m_texture)
		{
		case FrameBufferNormalColor:
            textureId = m_bufferSet->NormalPass.TextureId;
			break;
		case FrameBufferNormalDepth:
            textureId = m_bufferSet->NormalPass.DepthTextureId;
			break;
		case FrameBufferDeferredLightmap:
            textureId = m_bufferSet->DeferredLightmap.TextureId;
			break;
/*
		case MyShadowmap:
			textureId = CurLight->ShadowBuffer.DepthTextureId;
			break;
		case MyInnerShadowMap:
			textureId = CurLight->ShadowInnerBuffer.DepthTextureId;
			break;
*/
		case FrameBufferScene:
            textureId = m_bufferSet->ScenePass.TextureId;
			break;
		case Bloom:
            textureId = m_bufferSet->Bloom.TextureId;
			break;
		case BloomB:
            textureId = m_bufferSet->BloomB.TextureId;
			break;
		case FbSsaoPrepare:
            textureId = m_bufferSet->SsaoPrepare.TextureId;
			break;
		case FbSsaoPerform:
            textureId = m_bufferSet->SsaoPerform.TextureId;
			break;
		case FbSsaoBlur:
            textureId = m_bufferSet->SsaoBlur.TextureId;
			break;
/*
		case FbCubemap1:
			textureId = CurCubeMap->FrameBuffers[0].TextureId;
			break;
		case FbCubemap2:
			textureId = CurCubeMap->FrameBuffers[1].TextureId;
			break;
		case FbCubemap3:
			textureId = CurCubeMap->FrameBuffers[2].TextureId;
			break;
		case FbCubemap4:
			textureId = CurCubeMap->FrameBuffers[3].TextureId;
			break;
		case FbCubemap5:
			textureId = CurCubeMap->FrameBuffers[4].TextureId;
			break;
		case FbCubemap6:
			textureId = CurCubeMap->FrameBuffers[5].TextureId;
			break;
*/
		case FbDefReflections:
            textureId = m_bufferSet->ReflectionPass.TextureId;
			break;
		default:
			break;
		}

        if(textureId != GLUINT_MAX && ShaderData::HasUniform(m_target))
		{
            glActiveTexture(GL_TEXTURE0 + m_sv->getTextureUnitCount());
			glBindTexture(GL_TEXTURE_2D, textureId);
            ShaderData::Uniform1i(m_target, m_sv->getTextureUnitCount());
            m_sv->increaseTextureUnitCount();
		}
	}
}
