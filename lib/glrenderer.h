/*
  Q Light Controller Plus
  glrenderer.h

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

#ifndef GLRENDERER_H
#define GLRENDERER_H

#include "GL/glew.h"

//#include <assimp/Importer.hpp>
#include <assimp/cimport.h>
#include <assimp/scene.h>

#include <QList>

class GLRenderer
{
public:
    GLRenderer();

    static void set_float4(float f[], float a, float b, float c, float d);
    static void color4_to_float4(const aiColor4D *c, float f[]);

    // Vector functions
    void crossProduct(float *a, float *b, float *res);
    void normalize(float *a);

    // Matrix functions
    void pushMatrix();
    void popMatrix();
    void setIdentityMatrix(float *mat, int size);
    void multMatrix(float *a, float *b);
    void setTranslationMatrix(float *mat, float x, float y, float z);
    void setScaleMatrix(float *mat, float sx, float sy, float sz);
    void setRotationMatrix(float *mat, float angle, float x, float y, float z);

    void scale(float x, float y, float z);
    void rotate(float angle, float x, float y, float z);
    void translate(float x, float y, float z);
    void setModelMatrix();

    void setCamera(float posX, float posY, float posZ, float lookAtX, float lookAtY, float lookAtZ);

private:
    // Uniform Buffer for Matrices
    // this buffer will contain 3 matrices: projection, view and model
    // each matrix is a float array with 16 components
    GLuint matricesUniBuffer;

    // Model Matrix (part of the OpenGL Model View Matrix)
    float modelMatrix[16];

    // For push and pop matrix
    QList<float *> matrixStack;
};

#endif // GLRENDERER_H
