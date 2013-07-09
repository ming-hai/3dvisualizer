/*
  Q Light Controller Plus
  scenefilter.h

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

#ifndef SCENEFILTER_H
#define SCENEFILTER_H

#include "shaderdata.h"
#include "framebufferdata.h"
#include "framebuffertexture.h"

class SceneViewer;

class SceneFilter
{
public:
    SceneFilter(SceneViewer *scv);
    ~SceneFilter(void);
    bool bind(enum DrawingPass pass);

    void setShader(ShaderData *shader);
    void addFBTexture(enum Uniforms target, QString textureName);
    //static MaterialData* FromXml(char* source);

private:
    SceneViewer *m_scv;
    QString Name;
    bool isTransparent;

    QList<FramebufferTexture *> m_textures;
    ShaderData* m_shader;
    ShaderData* m_deferredShader;
    ShaderData* m_shadowShader;
};

#endif

