/*
  Q Light Controller Plus
  shaderdata.h

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

#ifndef SHADERDATA_H
#define SHADERDATA_H

#include <QString>

#include "GL/glew.h"
#include "GL/glut.h"
#include "math_3d.h"

//Don't forget to update UniformsStrings
enum Uniforms
{
	UniformsNull,
	TexDiffuse,
	TexDiffuse2,
	TexDiffuse3,
	TexDiffuse4,
	TexDiffuse5,
	TexDiffuse6,
	TexNormal,
	TexShadingInfo,
	TexDepth,
	TexDeffLightmap,
	TexShadowMap,
	TexInnerShadowMap,
	TexDeffReflections,
	MatShadowProjection,
	MatInnerShadowProjection,
    MatModelView,
    MatModelRotation,
    MatViewProjection,
	VecOrigin,
	VecSize,
	VecDirection,
	VecRenderSize,
	VecEyePos,
	VecEyeFwd,
	VecEyeRight,
	VecEyeUp,
	VecLight,
	VecAmbient,
	FltNear,
	FltFar,
	FltAspect,
	FltAlpha,
    FltState
};

enum AttributeIndices
{
	AttrPosition,
	AttrNormal,
	AttrTexCoord,
	AttrTangent,
	AttrBiNormal,
	AttrAlpha
};

enum DataType
{
	DataType1f,
	DataTypeMat4,
	DataTypeVector3
};

enum DrawingPass
{
    DrawingPassSolid,
    DrawingPassSolidForced,
    DrawingPassTransparent,
    DrawingPassDeffered,
    DrawingPassShadow
};

struct UniformInsert
{
    enum Uniforms uniform;
	enum DataType type;
	void* data;

    inline UniformInsert(){}

	inline UniformInsert(
        enum Uniforms mUniform,
		enum DataType mType,
		void* mData)
	{
        uniform = mUniform;
		type = mType;
		data = mData;
    }
};

//extern int UniformCount;

class ShaderData
{
public:
	GLuint shaderprogram;
    ShaderData(const char* vertexShader,const char* fragmentShader);
	~ShaderData(void);
	void Bind(void);
    QString VertexName;
    QString FragmentName;
    static QString fileToBuffer(QString filename);
    static ShaderData* FromPlainText(QString vertexSource, QString fragmentSource);
	static void Uniform1i(enum Uniforms, GLint i);
	static void Uniform1f(enum Uniforms, GLfloat f);
    static void Uniform3fv(enum Uniforms, Vector3f const & vec);
    static void Uniform2fv(enum Uniforms, Vector2f const & vec);
    static void UniformMatrix4fv(enum Uniforms target, Matrix4f const & matrix);
	static void UniformMatrix4fv(enum Uniforms target, GLfloat* matrix );
    static void ParseUniformInserts(QList<UniformInsert *> list);
    static bool HasUniform(enum Uniforms);
private:
	GLint* UniformLocations;
	void generateLocations();
	GLint getLocation(enum Uniforms target);
	GLuint vertexshader, fragmentshader;
	GLchar *vertexsource, *fragmentsource;
};

enum Uniforms GetUniformEnum(char*);

#endif // SHADERDATA_H
