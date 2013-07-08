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

#include <QList>
#include <QFile>
#include <QDebug>
#include <QTextStream>

#include "shaderdata.h"

//Don't forget to update enum Uniforms
static QString const UniformsStrings[] = {
	"NULL",
	"Diffuse",
	"Diffuse2",
	"Diffuse3",
	"Diffuse4",
	"Diffuse5",
	"Diffuse6",
	"Normal",
	"ShadingInfo",
	"Depth",
	"DefferedLightmap",
	"Shadowmap",
	"InnerShadowmap",
	"Reflections",
	"ShadowProjection",
	"InnerShadowProjection",
	"ModelView",
    "ModelRotation",
	"ViewProjection",
	"Origin",
	"Size",
	"Direction",
	"RenderSize",
	"EyePos",
	"EyeFwd",
	"EyeRight",
	"EyeUp",
	"Light",
	"Ambient",
	"NearZ",
	"FarZ",
	"Aspect",
	"Alpha",
	"State"
};

static int const UniformCount = sizeof(UniformsStrings)/sizeof(UniformsStrings[0]);
static ShaderData *curShader = NULL;

//generates a list with all existing Uniforms and their locations
void ShaderData::generateLocations()
{
	UniformLocations = new GLint[UniformCount];
	for (int i = 0; i < UniformCount; i++)
        UniformLocations[i] = glGetUniformLocation(shaderprogram, UniformsStrings[i].toAscii().data());
}

ShaderData::ShaderData(const char* vertexsource,const char* fragmentsource)
{
	char *vertexInfoLog;
    char *fragmentInfoLog;
	int IsCompiled_VS, IsCompiled_FS;
	int maxLength;

    /* Create an empty vertex shader handle */
    vertexshader = glCreateShader(GL_VERTEX_SHADER);
 
    /* Send the vertex shader source code to GL */
    /* Note that the source code is NULL character terminated. */
    /* GL will automatically detect that therefore the length info can be 0 in this case (the last parameter) */
    glShaderSource(vertexshader, 1, (const GLchar**)&vertexsource, 0);
 
    /* Compile the vertex shader */
    glCompileShader(vertexshader);
 
    glGetShaderiv(vertexshader, GL_COMPILE_STATUS, &IsCompiled_VS);
	if(IsCompiled_VS == false)
    {
       glGetShaderiv(vertexshader, GL_INFO_LOG_LENGTH, &maxLength);
 
       /* The maxLength includes the NULL character */
       vertexInfoLog = (char *)malloc(maxLength);
 
       glGetShaderInfoLog(vertexshader, maxLength, &maxLength, vertexInfoLog);
 
       /* Handle the error in an appropriate way such as displaying a message or writing to a log file. */
       qDebug() << "Compiled Vertex Shader: " << vertexInfoLog;

       /* In this simple program, we'll just leave */
       free(vertexInfoLog);
       return;
    }
    else
    {
        qDebug() << "Compiled Vertex Successfully";
    }
 
    /* Create an empty fragment shader handle */
    fragmentshader = glCreateShader(GL_FRAGMENT_SHADER);
 
    /* Send the fragment shader source code to GL */
    /* Note that the source code is NULL character terminated. */
    /* GL will automatically detect that therefore the length info can be 0 in this case (the last parameter) */
    glShaderSource(fragmentshader, 1, (const GLchar**)&fragmentsource, 0);
 
    /* Compile the fragment shader */
    glCompileShader(fragmentshader);
 
    glGetShaderiv(fragmentshader, GL_COMPILE_STATUS, &IsCompiled_FS);
    if(IsCompiled_FS == false)
    {
       glGetShaderiv(fragmentshader, GL_INFO_LOG_LENGTH, &maxLength);
 
       /* The maxLength includes the NULL character */
       fragmentInfoLog = (char *)malloc(maxLength);
 
       glGetShaderInfoLog(fragmentshader, maxLength, &maxLength, fragmentInfoLog);
 
       /* Handle the error in an appropriate way such as displaying a message or writing to a log file. */
       qDebug() << "Compiled Vertex Shader: " << fragmentInfoLog;

       /* In this simple program, we'll just leave */
       free(fragmentInfoLog);
       return;
    }
    else
    {
        qDebug() << "Compiled Fragment Shader Successfully";
    }

    /* Assign our program handle a "name" */
    shaderprogram = glCreateProgram();
 
    /* Attach our shaders to our program */
    glAttachShader(shaderprogram, vertexshader);
    glAttachShader(shaderprogram, fragmentshader);

	glBindAttribLocation(shaderprogram, AttrPosition, "in_Position");
    glBindAttribLocation(shaderprogram, AttrNormal, "in_Normal");
	glBindAttribLocation(shaderprogram, AttrTexCoord, "in_TexCoord");
	glBindAttribLocation(shaderprogram, AttrTangent, "in_Tangent");
	glBindAttribLocation(shaderprogram, AttrBiNormal, "in_BiNormal");
	glBindAttribLocation(shaderprogram, AttrAlpha, "in_Alpha");

    glLinkProgram(shaderprogram);

    generateLocations();

	glUseProgram(0);
}

QString ShaderData::fileToBuffer(QString filename)
{
    QFile file(filename);
    QString content;
    if (file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QTextStream in(&file);
        content = in.readAll();
        return content;
    }

    return "";
}

ShaderData* ShaderData::FromPlainText(QString vertexSource, QString fragmentSource)
{
    /* Read our shaders into the appropriate buffers */
    QString vertexsource = fileToBuffer(vertexSource);
    QString fragmentsource = fileToBuffer(fragmentSource);

    ShaderData* Shader = new ShaderData(vertexsource.toStdString().c_str(), fragmentsource.toStdString().c_str());
    Shader->VertexName = vertexSource;
    Shader->FragmentName = fragmentSource;

	return Shader;
}

void ShaderData::Uniform1i(enum Uniforms target, GLint i)
{
    GLint location = curShader->getLocation(target);
	glUniform1i(location, i);
}

void ShaderData::Uniform1f(enum Uniforms target, GLfloat f)
{
    GLint location = curShader->getLocation(target);
	glUniform1f(location, f);
}

void ShaderData::Uniform3fv(enum Uniforms target, const Vector3f &vec)
{
    GLint location = curShader->getLocation(target);
    glUniform3f(location, vec.x, vec.y, vec.z);
}

void ShaderData::Uniform2fv(enum Uniforms target, const Vector2f &vec)
{
    GLint location = curShader->getLocation(target);
    glUniform2f(location, vec.x, vec.y);
}

void ShaderData::UniformMatrix4fv(enum Uniforms target, GLfloat* matrix )
{
    GLint location = curShader->getLocation(target);
    glUniformMatrix4fv(location, 1,GL_TRUE, matrix);
}

void ShaderData::UniformMatrix4fv(enum Uniforms target, const Matrix4f &matrix)
{
    GLint location = curShader->getLocation(target);
    glUniformMatrix4fv(location, 1,GL_TRUE, (const GLfloat*)matrix.m_matrix);

//    qDebug() << Q_FUNC_INFO << "INSERTING MATRIX";
//    qDebug() << matrix.m[0][0] << matrix.m[0][1] << matrix.m[0][2] << matrix.m[0][3];
//    qDebug() << matrix.m[1][0] << matrix.m[1][1] << matrix.m[1][2] << matrix.m[1][3];
//    qDebug() << matrix.m[2][0] << matrix.m[2][1] << matrix.m[2][2] << matrix.m[2][3];
//    qDebug() << matrix.m[3][0] << matrix.m[3][1] << matrix.m[3][2] << matrix.m[3][3];
}

GLint ShaderData::getLocation(enum Uniforms target)
{
	return UniformLocations[target];
}

bool ShaderData::HasUniform(enum Uniforms target)
{
    return curShader->UniformLocations[target] != -1;
}

void ShaderData::ParseUniformInserts(QList<UniformInsert*> list)
{
    if(list.count() == 0)
        return;

    foreach(UniformInsert *insert, list)
	{
		switch (insert->type)
		{
		case DataType1f:
            Uniform1f(insert->uniform,*(GLfloat*)insert->data);
			break;
		case DataTypeMat4:
            UniformMatrix4fv(insert->uniform, *(Matrix4f*)insert->data);
			break;
		case DataTypeVector3:
            Uniform3fv(insert->uniform,*(Vector3f*)insert->data);
			break;
		default:
			break;
		}
	}

}

ShaderData::~ShaderData(void)
{
	delete[] UniformLocations;
	glDetachShader(shaderprogram, vertexshader);
    glDetachShader(shaderprogram, fragmentshader);
    glDeleteProgram(shaderprogram);
    glDeleteShader(vertexshader);
    glDeleteShader(fragmentshader);
}

void ShaderData::bind(void)
{
    curShader = this;
    glUseProgram(shaderprogram);
}

enum Uniforms GetUniformEnum(char* Name)
{
	for (int i = 0; i < UniformCount; i++)
	{
        if(UniformsStrings[i] == Name)
			return (Uniforms)i;
	}
	return UniformsNull;
}
