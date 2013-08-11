/*
  Q Light Controller Plus
  spotlight.h

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


#ifndef SPOTLIGHT_H
#define SPOTLIGHT_H

#include <GL/glew.h>
#include <QColor>

#include "math_3d.h"
#include "shader.h"
#include "scenenode.h"
#include "fborendertexture.h"

class Spotlight
{
public:
    Spotlight(ViewPort *view, FBORenderTexture *fbo);

    void render();

private:
    Shader m_shader;
    ViewPort *m_view; // Reference to the main viewport
    SceneNode *m_lightVolume;
    FBORenderTexture *m_fbo;

    // Shader uniform locations
    GLuint m_diffuseLoc; // Diffuse texture handle for the shader
    GLuint m_positionLoc; // Position texture handle for the shader
    GLuint m_normalsLoc; // Normals texture handle for the shader

    GLuint m_lightPosLoc;
    GLuint m_lightColorLoc;
    GLuint m_radiusLoc;
    GLuint m_intensityLoc;
    GLuint m_directionLoc;
    GLuint m_cosCutOffLoc;

    Vector3f m_position;
    Vector3f m_direction;
    float m_radius;
    float m_intensity;
    QColor m_color;
    float m_angleFrac; // a fraction of Pi

};

#endif // SPOTLIGHT_H
