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

//#include "Base.h"
#include "shaderdata.h"
//#include "GenFunc.h"
#include "texturedata.h"

//extern vec2 CurRenderDim;

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

class BufferSet
{
public:
	// ShadingOptions
	bool EnableSsao;
	bool EnableBloom;
	int SizeX;
	int SizeY;

	// Used access result
	FrameBufferData* OutBuffer;
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

extern BufferSet* CurrentBufferSet;

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

struct FbTextureBinder : TextureData
{
	enum FrameBuffer Texture;
	enum Uniforms Target;

	FbTextureBinder(char* target, char* texture);
    virtual void bind();
private:
    int CurTexUnit;
};

#endif // FRAMEBUFFERDATA_H
