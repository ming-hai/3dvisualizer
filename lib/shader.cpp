#include "GL/glew.h"
#include "shader.h"

#include <QDir>
#include <QDebug>
#include <QFile>
#include <QTextStream>

#define SHADERS_FOLDER "shaders"

QString Shader::fileToBuffer(QString filename)
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

/**
*	Constructor
*/
Shader::Shader(QString _sVSFileName, QString _sFSFileName)
	: m_VSFileName(_sVSFileName)
	, m_FSFileName(_sFSFileName)
{
    QString vertexBuf = fileToBuffer(QString(SHADERS_FOLDER) + QDir::separator() + _sVSFileName);
    QString fragmentBuf = fileToBuffer(QString(SHADERS_FOLDER) + QDir::separator() + _sFSFileName);

    //qDebug() << "VERTEX SHADER: " << vertexBuf;
    //qDebug() << "FRAGMENT SHADER: " << fragmentBuf;
    std::string tmpVertex = vertexBuf.toStdString();
    std::string tmpFrag = fragmentBuf.toStdString();
    const char* vertexsource = tmpVertex.c_str();
    const char* fragmentsource = tmpFrag.c_str();

    char *vertexInfoLog;
    char *fragmentInfoLog;
    int IsCompiled_VS, IsCompiled_FS;
    int maxLength;

    /* Create an empty vertex shader handle */
    m_vertexshader = glCreateShader(GL_VERTEX_SHADER);

    /* Send the vertex shader source code to GL */
    /* Note that the source code is NULL character terminated. */
    /* GL will automatically detect that therefore the length info can be 0 in this case (the last parameter) */
    glShaderSource(m_vertexshader, 1, (const GLchar**)&vertexsource, 0);

    /* Compile the vertex shader */
    glCompileShader(m_vertexshader);

    glGetShaderiv(m_vertexshader, GL_COMPILE_STATUS, &IsCompiled_VS);
    if(IsCompiled_VS == false)
    {
       glGetShaderiv(m_vertexshader, GL_INFO_LOG_LENGTH, &maxLength);

       /* The maxLength includes the NULL character */
       vertexInfoLog = (char *)malloc(maxLength);

       glGetShaderInfoLog(m_vertexshader, maxLength, &maxLength, vertexInfoLog);

       /* Handle the error in an appropriate way such as displaying a message or writing to a log file. */
       qDebug() << "Compiled Vertex Shader: " << vertexInfoLog;

       /* In this simple program, we'll just leave */
       free(vertexInfoLog);
       return;
    }
    else
    {
        qDebug() << _sVSFileName << "compiled successfully";
    }

    /* Create an empty fragment shader handle */
    m_fragmentshader = glCreateShader(GL_FRAGMENT_SHADER);

    /* Send the fragment shader source code to GL */
    /* Note that the source code is NULL character terminated. */
    /* GL will automatically detect that therefore the length info can be 0 in this case (the last parameter) */
    glShaderSource(m_fragmentshader, 1, (const GLchar**)&fragmentsource, 0);

    /* Compile the fragment shader */
    glCompileShader(m_fragmentshader);

    glGetShaderiv(m_fragmentshader, GL_COMPILE_STATUS, &IsCompiled_FS);
    if(IsCompiled_FS == false)
    {
       glGetShaderiv(m_fragmentshader, GL_INFO_LOG_LENGTH, &maxLength);

       /* The maxLength includes the NULL character */
       fragmentInfoLog = (char *)malloc(maxLength);

       glGetShaderInfoLog(m_fragmentshader, maxLength, &maxLength, fragmentInfoLog);

       /* Handle the error in an appropriate way such as displaying a message or writing to a log file. */
       qDebug() << "Compiled Fragment Shader: " << fragmentInfoLog;

       /* In this simple program, we'll just leave */
       free(fragmentInfoLog);
       return;
    }
    else
    {
        qDebug() << _sFSFileName << "compiled successfully";
    }

    /* Assign our program handle a "name" */
    m_shaderProg = glCreateProgram();

    /* Attach our shaders to our program */
    glAttachShader(m_shaderProg, m_vertexshader);
    glAttachShader(m_shaderProg, m_fragmentshader);

    glLinkProgram(m_shaderProg);

    glUseProgram(0);
}

void Shader::bind()
{
    glUseProgram(m_shaderProg);
}

