#include "fborendertexture.h"
#include "deferredrendering.h"

/**
*	Create the deferred rendering object. I have hardcoded the shader's name here.
*/
DeferredRendering::DeferredRendering(int _dWidth, int _dHeight, FBORenderTexture* fboRenderTexture, ViewPort *view)
    : m_shader("shaders/deferredRendering.vert", "shaders/deferredRendering.frag")
	, m_fboRenderTexture(fboRenderTexture)
    , m_view(view)
	, m_width(_dWidth)
	, m_height(_dHeight)
{	
	// Get the handles from the shader
    m_diffuseLoc = glGetUniformLocation(m_shader.m_shaderProg, "tDiffuse");
    m_positionLoc = glGetUniformLocation(m_shader.m_shaderProg, "tPosition");
    m_normalsLoc = glGetUniformLocation(m_shader.m_shaderProg, "tNormals");

    m_screenQuad = new SceneNode(view);
    QString screenQuad =
            "mtllib floor.mtl\n"
            "o Plane\n"
            "v 1.000000 -1.000000 0.000000\n"
            "v -1.000000 -1.000000 0.000000\n"
            "v 1.000000 1.000000 0.000000\n"
            "v -1.000000 1.000000 0.000000\n"
            "vt 0.000000 1.000000\n"
            "vt 1.000000 1.000000\n"
            "vt 1.000000 0.000000\n"
            "vt 0.000000 0.000000\n"
            "vn 0.000000 0.000000 1.000000\n"
            "usemtl\n"
            "s off\n"
            "f 2/1/1 1/2/1 3/3/1\n"
            "f 2/1/1 3/3/1 4/4/1\n";
    m_screenQuad->loadModelFromBuffer(screenQuad);
    m_screenQuad->setShader(&m_shader);
}

/**
*	Render the big quad with the deferred rendering shader on it
*/
void DeferredRendering::render()
{
    glUseProgram(m_shader.m_shaderProg);

	glActiveTextureARB(GL_TEXTURE0_ARB);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, m_fboRenderTexture->getDiffuseTexture());
    glUniform1iARB ( m_diffuseLoc, 0 );
	
	glActiveTextureARB(GL_TEXTURE1_ARB);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, m_fboRenderTexture->getPositionTexture());
    glUniform1iARB ( m_positionLoc, 1 );
	
	glActiveTextureARB(GL_TEXTURE2_ARB);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, m_fboRenderTexture->getNormalsTexture());
    glUniform1iARB ( m_normalsLoc, 2 );

	// Render the quad
    m_screenQuad->render(false);

	// Reset OpenGL state
	glActiveTextureARB(GL_TEXTURE0_ARB);
	glDisable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);

	glActiveTextureARB(GL_TEXTURE1_ARB);
	glDisable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);

	glActiveTextureARB(GL_TEXTURE2_ARB);
	glDisable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);

    glUseProgram(0);
}
