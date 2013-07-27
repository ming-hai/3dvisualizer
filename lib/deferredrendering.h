#pragma once
#include <GL/glew.h>

#include "fborendertexture.h"
#include "scenenode.h"
#include "viewport.h"
#include "shader.h"

/**
*	This object is used to render a big screen sized quad with the deferred rendering shader applied on it.
*/
class DeferredRendering
{
public:
	// Ctors/Dtors
    DeferredRendering(int width, int height, FBORenderTexture* fboRenderTexture, ViewPort *view);

	// Methods
	void	render();

private:
	// Variables
    Shader 		        m_shader; // Deferred rendering shader
	FBORenderTexture*	m_fboRenderTexture; // A pointer to the FBO render texture that contains diffuse, normals and positions
    ViewPort*           m_view;
    SceneNode*          m_screenQuad;

	unsigned int		m_width; // width
	unsigned int		m_height; // height

    GLuint				m_diffuseLoc; // Diffuse texture handle for the shader
    GLuint				m_positionLoc; // Position texture handle for the shader
    GLuint				m_normalsLoc; // Normals texture handle for the shader
};
