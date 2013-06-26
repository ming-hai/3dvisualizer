/*
  Q Light Controller Plus
  texturedata.h

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

#ifndef TEXTUREDATA_H
#define TEXTUREDATA_H

#include <QString>
#include <QColor>

#include "shaderdata.h"

#define GLUINT_MAX (GLuint)(1 << 31)

class SceneViewer;

class TextureData
{
public:
    TextureData(SceneViewer* sv);
    ~TextureData();

    void initData();
    bool loadMaterial(QColor color);
    bool loadTexture(QString path);

	TextureData* SetTarget(enum Uniforms);
    virtual void bind(void);

private:
    SceneViewer *m_sv;

    enum Uniforms Target;
    QString Name;
    GLuint textureId;
};

#endif // TEXTUREDATA_H
