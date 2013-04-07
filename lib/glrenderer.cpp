/*
  Q Light Controller Plus
  glrenderer.cpp

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

#include "glrenderer.h"

#define MatricesUniBufferSize sizeof(float) * 16 * 3
#define ProjMatrixOffset 0
#define ViewMatrixOffset sizeof(float) * 16
#define ModelMatrixOffset sizeof(float) * 16 * 2
#define MatrixSize sizeof(float) * 16

GLRenderer::GLRenderer()
{
}

static inline float DegToRad(float degrees)
{
    return (float)(degrees * (M_PI / 180.0f));
}

void GLRenderer::set_float4(float f[], float a, float b, float c, float d)
{
    f[0] = a;
    f[1] = b;
    f[2] = c;
    f[3] = d;
}

void GLRenderer::color4_to_float4(const aiColor4D *c, float f[])
{
    f[0] = c->r;
    f[1] = c->g;
    f[2] = c->b;
    f[3] = c->a;
}

// ----------------------------------------------------
// VECTOR STUFF
//
// res = a cross b;

void GLRenderer::crossProduct( float *a, float *b, float *res) {

    res[0] = a[1] * b[2]  -  b[1] * a[2];
    res[1] = a[2] * b[0]  -  b[2] * a[0];
    res[2] = a[0] * b[1]  -  b[0] * a[1];
}


// Normalize a vec3
void GLRenderer::normalize(float *a) {

    float mag = sqrt(a[0] * a[0]  +  a[1] * a[1]  +  a[2] * a[2]);

    a[0] /= mag;
    a[1] /= mag;
    a[2] /= mag;
}

// ----------------------------------------------------
// MATRIX STUFF
//

// Push and Pop for modelMatrix

void GLRenderer::pushMatrix()
{
    float *aux = (float *)malloc(sizeof(float) * 16);
    memcpy(aux, modelMatrix, sizeof(float) * 16);
    matrixStack.push_back(aux);
}

void GLRenderer::popMatrix()
{
    float *m = matrixStack[matrixStack.size()-1];
    memcpy(modelMatrix, m, sizeof(float) * 16);
    matrixStack.pop_back();
    free(m);
}

// sets the square matrix mat to the identity matrix,
// size refers to the number of rows (or columns)
void GLRenderer::setIdentityMatrix( float *mat, int size)
{
    // fill matrix with 0s
    for (int i = 0; i < size * size; ++i)
            mat[i] = 0.0f;

    // fill diagonal with 1s
    for (int i = 0; i < size; ++i)
        mat[i + i * size] = 1.0f;
}


//
// a = a * b;
//
void GLRenderer::multMatrix(float *a, float *b)
{
    float res[16];

    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            res[j*4 + i] = 0.0f;
            for (int k = 0; k < 4; ++k) {
                res[j*4 + i] += a[k*4 + i] * b[j*4 + k];
            }
        }
    }
    memcpy(a, res, 16 * sizeof(float));

}


// Defines a transformation matrix mat with a translation
void GLRenderer::setTranslationMatrix(float *mat, float x, float y, float z)
{
    setIdentityMatrix(mat,4);
    mat[12] = x;
    mat[13] = y;
    mat[14] = z;
}

// Defines a transformation matrix mat with a scale
void GLRenderer::setScaleMatrix(float *mat, float sx, float sy, float sz)
{
    setIdentityMatrix(mat,4);
    mat[0] = sx;
    mat[5] = sy;
    mat[10] = sz;
}

// Defines a transformation matrix mat with a rotation
// angle alpha and a rotation axis (x,y,z)
void GLRenderer::setRotationMatrix(float *mat, float angle, float x, float y, float z)
{
    float radAngle = DegToRad(angle);
    float co = cos(radAngle);
    float si = sin(radAngle);
    float x2 = x*x;
    float y2 = y*y;
    float z2 = z*z;

    mat[0] = x2 + (y2 + z2) * co;
    mat[4] = x * y * (1 - co) - z * si;
    mat[8] = x * z * (1 - co) + y * si;
    mat[12]= 0.0f;

    mat[1] = x * y * (1 - co) + z * si;
    mat[5] = y2 + (x2 + z2) * co;
    mat[9] = y * z * (1 - co) - x * si;
    mat[13]= 0.0f;

    mat[2] = x * z * (1 - co) - y * si;
    mat[6] = y * z * (1 - co) + x * si;
    mat[10]= z2 + (x2 + y2) * co;
    mat[14]= 0.0f;

    mat[3] = 0.0f;
    mat[7] = 0.0f;
    mat[11]= 0.0f;
    mat[15]= 1.0f;

}

// ----------------------------------------------------
// Model Matrix
//
// Copies the modelMatrix to the uniform buffer


void GLRenderer::setModelMatrix()
{
    glBindBuffer(GL_UNIFORM_BUFFER,matricesUniBuffer);
    glBufferSubData(GL_UNIFORM_BUFFER,
                       ModelMatrixOffset, MatrixSize, modelMatrix);
    glBindBuffer(GL_UNIFORM_BUFFER,0);
}

// The equivalent to glTranslate applied to the model matrix
void GLRenderer::translate(float x, float y, float z)
{
    float aux[16];

    setTranslationMatrix(aux,x,y,z);
    multMatrix(modelMatrix,aux);
    setModelMatrix();
}

// The equivalent to glRotate applied to the model matrix
void GLRenderer::rotate(float angle, float x, float y, float z)
{
    float aux[16];

    setRotationMatrix(aux,angle,x,y,z);
    multMatrix(modelMatrix,aux);
    setModelMatrix();
}

// The equivalent to glScale applied to the model matrix
void GLRenderer::scale(float x, float y, float z)
{
    float aux[16];

    setScaleMatrix(aux,x,y,z);
    multMatrix(modelMatrix,aux);
    setModelMatrix();
}

// ----------------------------------------------------
// View Matrix
//
// Computes the viewMatrix and stores it in the uniform buffer
//
// note: it assumes the camera is not tilted,
// i.e. a vertical up vector along the Y axis (remember gluLookAt?)
//

void GLRenderer::setCamera(float posX, float posY, float posZ,
               float lookAtX, float lookAtY, float lookAtZ) {

    float dir[3], right[3], up[3];

    up[0] = 0.0f;	up[1] = 1.0f;	up[2] = 0.0f;

    dir[0] =  (lookAtX - posX);
    dir[1] =  (lookAtY - posY);
    dir[2] =  (lookAtZ - posZ);
    normalize(dir);

    crossProduct(dir,up,right);
    normalize(right);

    crossProduct(right,dir,up);
    normalize(up);

    float viewMatrix[16],aux[16];

    viewMatrix[0]  = right[0];
    viewMatrix[4]  = right[1];
    viewMatrix[8]  = right[2];
    viewMatrix[12] = 0.0f;

    viewMatrix[1]  = up[0];
    viewMatrix[5]  = up[1];
    viewMatrix[9]  = up[2];
    viewMatrix[13] = 0.0f;

    viewMatrix[2]  = -dir[0];
    viewMatrix[6]  = -dir[1];
    viewMatrix[10] = -dir[2];
    viewMatrix[14] =  0.0f;

    viewMatrix[3]  = 0.0f;
    viewMatrix[7]  = 0.0f;
    viewMatrix[11] = 0.0f;
    viewMatrix[15] = 1.0f;

    setTranslationMatrix(aux, -posX, -posY, -posZ);

    multMatrix(viewMatrix, aux);

    glBindBuffer(GL_UNIFORM_BUFFER, matricesUniBuffer);
    glBufferSubData(GL_UNIFORM_BUFFER, ViewMatrixOffset, MatrixSize, viewMatrix);
    glBindBuffer(GL_UNIFORM_BUFFER,0);

    // set the model matrix to the identity Matrix
    setIdentityMatrix(modelMatrix,4);
}






