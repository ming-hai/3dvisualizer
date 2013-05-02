/*

	Copyright 2011 Etay Meiri

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "technique.h"
#include "gbuffer.h"

#include <QFile>
#include <QDebug>
#include <QTextStream>

QString Technique::fileToBuffer(QString filename)
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

void Technique::addShader(GLuint ShaderProgram, QString pShaderText, GLenum ShaderType)
{
    GLuint ShaderObj = glCreateShader(ShaderType);

    if (ShaderObj == 0)
    {
        qDebug() << "Error creating shader type " << ShaderType;
        return;
    }

    const GLchar* p[1];
    p[0] = pShaderText.toStdString().data();
    GLint Lengths[1];
    Lengths[0]= pShaderText.length();
    glShaderSource(ShaderObj, 1, p, Lengths);
    glCompileShader(ShaderObj);
    GLint success;
    glGetShaderiv(ShaderObj, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        GLchar InfoLog[1024];
        glGetShaderInfoLog(ShaderObj, 1024, NULL, InfoLog);
        qDebug() << "Error compiling shader " << m_shaderFile << "(type:" << ShaderType << ") - log:" << InfoLog;
        exit(1);
    }

    glAttachShader(ShaderProgram, ShaderObj);
}

void Technique::compileShaders(QString pFS, QString pVS)
{
    m_shaderProg = glCreateProgram();

    if (m_shaderProg == 0)
    {
        qDebug() << "Error creating shader program";
        return;
    }

    addShader(m_shaderProg, pVS, GL_VERTEX_SHADER);
    addShader(m_shaderProg, pFS, GL_FRAGMENT_SHADER);

    GLint Success = 0;
    GLchar ErrorLog[1024] = { 0 };

    glLinkProgram(m_shaderProg);
    glGetProgramiv(m_shaderProg, GL_LINK_STATUS, &Success);
    if (Success == 0)
    {
        glGetProgramInfoLog(m_shaderProg, sizeof(ErrorLog), NULL, ErrorLog);
        qDebug() << "Error linking shader program:" << ErrorLog;
        return;
    }

    glValidateProgram(m_shaderProg);
    glGetProgramiv(m_shaderProg, GL_VALIDATE_STATUS, &Success);
    if (!Success)
    {
        glGetProgramInfoLog(m_shaderProg, sizeof(ErrorLog), NULL, ErrorLog);
        qDebug() << "Invalid shader program:" << ErrorLog;
        return;
    }
}


Technique::Technique(QString shaderFile)
{
    m_shaderFile = shaderFile;
    m_shaderProg = 0;
}


Technique::~Technique()
{
    if (m_shaderProg != 0)
    {
        glDeleteProgram(m_shaderProg);
        m_shaderProg = 0;
    }
}

bool Technique::CompileProgram(const char* pProgram)
{
    QString FS = fileToBuffer(m_shaderFile + ".fs");
    QString VS = fileToBuffer(m_shaderFile + ".vs");

    compileShaders(FS, VS);

    return true;
}

void Technique::Enable()
{
    glUseProgram(m_shaderProg);
}


GLint Technique::GetUniformLocation(const char* pUniformName)
{
    GLuint Location = glGetUniformLocation(m_shaderProg, pUniformName);

    if (Location == INVALID_OGL_VALUE)
        qDebug() << "Warning! Unable to get the location of uniform " << pUniformName;

    return Location;
}

GLint Technique::GetProgramParam(GLint param)
{
    GLint ret;
    glGetProgramiv(m_shaderProg, param, &ret);
    return ret;
}
