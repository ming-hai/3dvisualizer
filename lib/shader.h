#pragma once
#include <QString>

/**
*	GLSLShaderData encapsulate all the GLSL data.<br>
*	Constructor initialize data from files.
*/
class Shader
{
public:	
	// Functions
	//------------------------------------------------------------
    Shader(QString sVSFileName, QString sFSFileName);

    static QString fileToBuffer(QString filename);

    void bind();

	// Variables
	//------------------------------------------------------------
    QString m_VSFileName; // Vertex shader filename
    QString m_FSFileName; // Fragment shader filename
    GLint m_shaderProg;
    GLuint m_vertexshader, m_fragmentshader;
};
