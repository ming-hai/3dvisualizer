/*
  Q Light Controller Plus
  framebufferdata.h

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

#ifndef FRAMEBUFFERDATA_H
#define FRAMEBUFFERDATA_H

#include <QColor>

#include "shaderdata.h"
#include "framebuffertexture.h"

class SceneViewer;

/*****************************************************************************
 * FrameBufferData class
 *****************************************************************************/

class FrameBufferData
{
public:
	//settings
	int SizeX;
	int SizeY;
    QColor Clearcolor;
	GLint DepthBufferFmt;
	GLint BufferFmt;
	bool UseColor;
    bool MultiSampling;

	//info
	GLuint TextureId;
	GLuint DepthTextureId;
	GLuint FboId;
	bool FboReady;

	//creation
	FrameBufferData(void);
	~FrameBufferData(void);
	void Initialize();
	virtual void Bind(bool clear);
    void SetMultiSampling(bool value);
};

/*****************************************************************************
 * DefaultFrameBuffer class
 *****************************************************************************/

class DefaultFrameBuffer : FrameBufferData
{
public:
    DefaultFrameBuffer();
    virtual void Bind(bool clear);

    void setSize(int w, int h);

private:
    int m_width;
    int m_height;
};

/*****************************************************************************
 * BufferSet class
 *****************************************************************************/

class BufferSet
{
public:
	// ShadingOptions
	bool EnableSsao;
	bool EnableBloom;
    int SizeX;
    int SizeY;

	// Used access result
    DefaultFrameBuffer* OutBuffer;
	FrameBufferData NormalPass;
	FrameBufferData ScenePass;
    FrameBufferData DeferredLightmap;
    FrameBufferData ReflectionPass;

	FrameBufferData Bloom;
	FrameBufferData BloomB;

	FrameBufferData SsaoPrepare;
	FrameBufferData SsaoPerform;
	FrameBufferData SsaoBlur;

	//Functions
	BufferSet();
	~BufferSet();
    void Initialize();
};

enum FrameBuffer
{
	FrameBufferNull,
	FrameBufferNormalColor,
	FrameBufferNormalDepth,
    FrameBufferDeferredLightmap,
	MyShadowmap,
	MyInnerShadowMap,
	FrameBufferScene,
	Bloom,
	BloomB,
	FbSsaoPrepare,
	FbSsaoPerform,
	FbSsaoBlur,
	FbCubemap1,
	FbCubemap2,
	FbCubemap3,
	FbCubemap4,
	FbCubemap5,
	FbCubemap6,
	FbDefReflections
};

/*****************************************************************************
 * FrameBufferTextureBinder class
 *****************************************************************************/

class FrameBufferTextureBinder : public FramebufferTexture
{
public:
    FrameBufferTextureBinder(enum Uniforms target, QString textureName, SceneViewer *sv);

    void bind();

private:
    SceneViewer* m_sv;
    enum FrameBuffer m_texture;
    enum Uniforms m_target;
    BufferSet* m_bufferSet;
};

#endif // FRAMEBUFFERDATA_H
