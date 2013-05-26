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
//#include "Sun.h"
//#include "CubeMap.h"

BufferSet* CurrentBufferSet;
Vector2f CurRenderDim;

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
		CurRenderDim.x = SizeX;
		CurRenderDim.y = SizeY;

		// clear buffers
		if(clear)
		{
            glClearColor(Clearcolor.red(), Clearcolor.green(), Clearcolor.blue(), Clearcolor.alpha());
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		}
	}
}

void FrameBufferData::SetMultiSampeling(bool value)
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

	DefferedLightmap.SizeX = SizeX;
	DefferedLightmap.SizeY = SizeY;
	DefferedLightmap.Initialize();

	ScenePass.SizeX = SizeX;
	ScenePass.SizeY = SizeY;
	ScenePass.Initialize();

	RefletionPass.SizeX = SizeX;
	RefletionPass.SizeY = SizeY;
	RefletionPass.Initialize();

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

    //OutBuffer = (FrameBufferData*)&defaultFramebuffer;
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

FbTextureBinder::FbTextureBinder(char* target, char* texture)
{
    CurTexUnit = 0;
	// Assign Uniform Tartget
	Target = GetUniformEnum(target);

	// Assign Texture Target
	Texture = FrameBufferNull;
	for (int i = 0; i < BufferNameCount; i++)
	{
        if(BufferNames[i] == texture)
		{
			Texture = (FrameBuffer)i;
			break;
		}
	}
}

void FbTextureBinder::Bind()
{
	if(ShaderData::HasUniform(Target))
	{
		GLuint textureId = -1;
		switch (Texture)
		{
		case FrameBufferNormalColor:
			textureId = CurrentBufferSet->NormalPass.TextureId;
			break;
		case FrameBufferNormalDepth:
			textureId = CurrentBufferSet->NormalPass.DepthTextureId;
			break;
		case FrameBufferDefferedLightmap:
			textureId = CurrentBufferSet->DefferedLightmap.TextureId;
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
			textureId = CurrentBufferSet->ScenePass.TextureId;
			break;
		case Bloom:
			textureId = CurrentBufferSet->Bloom.TextureId;
			break;
		case BloomB:
			textureId = CurrentBufferSet->BloomB.TextureId;
			break;
		case FbSsaoPrepare:
			textureId = CurrentBufferSet->SsaoPrepare.TextureId;
			break;
		case FbSsaoPerform:
			textureId = CurrentBufferSet->SsaoPerform.TextureId;
			break;
		case FbSsaoBlur:
			textureId = CurrentBufferSet->SsaoBlur.TextureId;
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
			textureId = CurrentBufferSet->RefletionPass.TextureId;
			break;
		default:
			break;
		}

        if(textureId != GLUINT_MAX && ShaderData::HasUniform(Target))
		{
			glActiveTexture(GL_TEXTURE0 + CurTexUnit);
			glBindTexture(GL_TEXTURE_2D, textureId);
			ShaderData::Uniform1i(Target, CurTexUnit);
			CurTexUnit++;
		}
	}
}
