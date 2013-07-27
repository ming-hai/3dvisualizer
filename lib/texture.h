/*
  Q Light Controller Plus
  texture.h

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

#ifndef TEXTURE_H
#define TEXTURE_H

#include <QObject>
#include <QString>
#include <QColor>
#include <GL/glew.h>

#define GLUINT_MAX (GLuint)(1 << 31)

class SceneViewer;

class Texture : public QObject
{
    Q_OBJECT
public:
    Texture(GLenum TextureTarget, QObject *parent = 0);
    ~Texture();

    void init();
    bool loadMaterial(QColor color);
    bool loadTexture(QString path);

    virtual void bind(GLenum TextureUnit);

private:
    SceneViewer *m_sv;

    GLenum m_textureTarget;
    QString Name;
    GLuint textureId;
};

#endif // TEXTUREDATA_H
