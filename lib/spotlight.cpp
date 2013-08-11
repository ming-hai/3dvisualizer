/*
  Q Light Controller Plus
  spotlight.cpp

  Copyright (C) Massimo Callegari

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

#include <qmath.h>

#include "spotlight.h"

Spotlight::Spotlight(ViewPort *view, FBORenderTexture *fbo)
    : m_shader("deferredRendering.vert", "spotlight.frag")
    , m_view(view)
    , m_fbo(fbo)
{
    m_position = Vector3f(0.0f, 5.0f, -9.0f);
    m_direction = Vector3f(0.0f,-1.0f, 0.0f);
    m_radius = 1.5f;
    m_intensity = 1.0f;
    m_angleFrac = 0.2f;

    m_color = QColor(0, 0, 255);

    m_diffuseLoc = glGetUniformLocation(m_shader.m_shaderProg, "tDiffuse");
    m_positionLoc = glGetUniformLocation(m_shader.m_shaderProg, "tPosition");
    m_normalsLoc = glGetUniformLocation(m_shader.m_shaderProg, "tNormals");

    m_lightPosLoc = glGetUniformLocation(m_shader.m_shaderProg, "lightPos");
    m_lightColorLoc = glGetUniformLocation(m_shader.m_shaderProg, "lightColor");
    m_radiusLoc = glGetUniformLocation(m_shader.m_shaderProg, "lightRadius");
    m_intensityLoc = glGetUniformLocation(m_shader.m_shaderProg, "lightIntensity");
    m_directionLoc = glGetUniformLocation(m_shader.m_shaderProg, "spotDir");
    m_cosCutOffLoc = glGetUniformLocation(m_shader.m_shaderProg, "spotCosCutoff");

    m_lightVolume = new SceneNode(m_view);
    m_lightVolume->loadModel(QString(MODELS_FOLDER) + QDir::separator() + "spotlight_cone.obj");
    m_lightVolume->setShader(&m_shader);
}

void Spotlight::render()
{
    glUseProgram(m_shader.m_shaderProg);

    glActiveTextureARB(GL_TEXTURE0_ARB);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, m_fbo->getDiffuseTexture());
    glUniform1iARB ( m_diffuseLoc, 0 );

    glActiveTextureARB(GL_TEXTURE1_ARB);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, m_fbo->getPositionTexture());
    glUniform1iARB ( m_positionLoc, 1 );

    glActiveTextureARB(GL_TEXTURE2_ARB);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, m_fbo->getNormalsTexture());
    glUniform1iARB ( m_normalsLoc, 2 );

    glUniform4f(m_lightPosLoc, m_position.x, m_position.y, m_position.z, 1.0f);
    glUniform3f(m_directionLoc, m_direction.x, m_direction.y, m_direction.z);
    glUniform4f(m_lightColorLoc, m_color.red(), m_color.green(), m_color.blue(), 1.0f);
    glUniform1f(m_radiusLoc, m_radius);
    glUniform1f(m_intensityLoc, m_intensity);
    glUniform1f(m_cosCutOffLoc, qCos(m_angleFrac * M_PI));

    m_lightVolume->render(false);

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

    glActiveTextureARB(GL_TEXTURE3_ARB);
    glDisable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);

    glUseProgram(0);
}
