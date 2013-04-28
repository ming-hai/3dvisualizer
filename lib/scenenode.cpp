/*
  Q Light Controller Plus
  scenenode.cpp

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
#include "scenenode.h"

#include <QDebug>

#define aisgl_min(x,y) (x<y?x:y)
#define aisgl_max(x,y) (y>x?y:x)

SceneNode::SceneNode(QObject *parent)
    : QObject(parent)
    , m_scene(NULL)
{
}

/*****************************************************************************
 * Node ID
 *****************************************************************************/

void SceneNode::setID(quint32 id)
{
    m_id = id;
    emit changed(m_id);
}

quint32 SceneNode::id() const
{
    return m_id;
}

quint32 SceneNode::invalidId()
{
    return UINT_MAX;
}

/*****************************************************************************
 * Node model
 *****************************************************************************/
void SceneNode::getBoundingBoxForNode(const aiNode *nd, aiVector3D *min, aiVector3D *max, aiMatrix4x4 *trafo)
{
    aiMatrix4x4 prev;
    unsigned int n = 0, t;

    prev = *trafo;
    aiMultiplyMatrix4(trafo,&nd->mTransformation);

    //int xx =   nd->mNumMeshes;

    for (; n < nd->mNumMeshes; ++n)
    {
        const aiMesh* mesh = m_scene->mMeshes[nd->mMeshes[n]];
        for (t = 0; t < mesh->mNumVertices; ++t)
        {
            aiVector3D tmp = mesh->mVertices[t];
            aiTransformVecByMatrix4(&tmp,trafo);

            min->x = aisgl_min(min->x,tmp.x);
            min->y = aisgl_min(min->y,tmp.y);
            min->z = aisgl_min(min->z,tmp.z);

            max->x = aisgl_max(max->x,tmp.x);
            max->y = aisgl_max(max->y,tmp.y);
            max->z = aisgl_max(max->z,tmp.z);
        }
    }

    for (n = 0; n < nd->mNumChildren; ++n)
    {
        getBoundingBoxForNode(nd->mChildren[n],min,max,trafo);
    }
    *trafo = prev;
}

void SceneNode::getBoundingBox(aiVector3D *min, aiVector3D *max)
{
    aiMatrix4x4 trafo;
    aiIdentityMatrix4(&trafo);

    min->x = min->y = min->z =  1e10f;
    max->x = max->y = max->z = -1e10f;
    getBoundingBoxForNode(m_scene->mRootNode,min,max,&trafo);
}

bool SceneNode::loadModel(QString path)
{
    m_scene = aiImportFile(path.toStdString().c_str(), aiProcessPreset_TargetRealtime_Quality);

    // If the import failed, report it
    if(m_scene == NULL)
    {
        qDebug() << Q_FUNC_INFO << "Cannot load " << path << " !!";
        return false;
    }

    getBoundingBox(&m_sceneMin, &m_sceneMax);
    m_sceneCenter.x = (m_sceneMin.x + m_sceneMax.x) / 2.0f;
    m_sceneCenter.y = (m_sceneMin.y + m_sceneMax.y) / 2.0f;
    m_sceneCenter.z = (m_sceneMin.z + m_sceneMax.z) / 2.0f;

    return true;
}

const aiScene *SceneNode::getModel() const
{
    return m_scene;
}

float SceneNode::getScale()
{
    float scale = m_sceneMax.x - m_sceneMin.x;
    scale = aisgl_max(m_sceneMax.y - m_sceneMin.y, scale);
    scale = aisgl_max(m_sceneMax.z - m_sceneMin.z, scale);
    scale = 1.f / scale;

    return scale;
}

aiVector3D SceneNode::getSceneCenter()
{
    return m_sceneMin;
}

// ----------------------------------------------------------------------------
void SceneNode::apply_material(const struct aiMaterial *mtl)
{
    float c[4];

    GLenum fill_mode;
    int ret1, ret2;
    aiColor4D diffuse;
    aiColor4D specular;
    aiColor4D ambient;
    aiColor4D emission;
    float shininess, strength;
    int two_sided;
    int wireframe;
    unsigned int max;

    GLRenderer::set_float4(c, 0.8f, 0.8f, 0.8f, 1.0f);
    if(AI_SUCCESS == aiGetMaterialColor(mtl, AI_MATKEY_COLOR_DIFFUSE, &diffuse))
        GLRenderer::color4_to_float4(&diffuse, c);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, c);

    GLRenderer::set_float4(c, 0.0f, 0.0f, 0.0f, 1.0f);
    if(AI_SUCCESS == aiGetMaterialColor(mtl, AI_MATKEY_COLOR_SPECULAR, &specular))
        GLRenderer::color4_to_float4(&specular, c);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, c);

    GLRenderer::set_float4(c, 0.2f, 0.2f, 0.2f, 1.0f);
    if(AI_SUCCESS == aiGetMaterialColor(mtl, AI_MATKEY_COLOR_AMBIENT, &ambient))
        GLRenderer::color4_to_float4(&ambient, c);
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, c);

    GLRenderer::set_float4(c, 0.0f, 0.0f, 0.0f, 1.0f);
    if(AI_SUCCESS == aiGetMaterialColor(mtl, AI_MATKEY_COLOR_EMISSIVE, &emission))
        GLRenderer::color4_to_float4(&emission, c);
    glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, c);

    max = 1;
    ret1 = aiGetMaterialFloatArray(mtl, AI_MATKEY_SHININESS, &shininess, &max);
    if(ret1 == AI_SUCCESS) {
        max = 1;
        ret2 = aiGetMaterialFloatArray(mtl, AI_MATKEY_SHININESS_STRENGTH, &strength, &max);
        if(ret2 == AI_SUCCESS)
            glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shininess * strength);
        else
            glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shininess);
    }
    else {
        glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 0.0f);
        GLRenderer::set_float4(c, 0.0f, 0.0f, 0.0f, 0.0f);
        glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, c);
    }

    max = 1;
    if(AI_SUCCESS == aiGetMaterialIntegerArray(mtl, AI_MATKEY_ENABLE_WIREFRAME, &wireframe, &max))
        fill_mode = wireframe ? GL_LINE : GL_FILL;
    else
        fill_mode = GL_FILL;
    glPolygonMode(GL_FRONT_AND_BACK, fill_mode);

    max = 1;
    if((AI_SUCCESS == aiGetMaterialIntegerArray(mtl, AI_MATKEY_TWOSIDED, &two_sided, &max)) && two_sided)
        glDisable(GL_CULL_FACE);
    else
        glEnable(GL_CULL_FACE);
}

void SceneNode::draw(const struct aiNode* nd)
{
    unsigned int i;
    unsigned int n = 0, t;
    if (nd == NULL)
        nd = m_scene->mRootNode;

    aiMatrix4x4 m = nd->mTransformation;

    // update transform
    //aiTransposeMatrix4(&m); <-- AssImp 3.0 ??
    m.Transpose();
    glPushMatrix();
    glMultMatrixf((float*)&m);

    // draw all meshes assigned to this node
    for (; n < nd->mNumMeshes; ++n)
    {
        const struct aiMesh* mesh = m_scene->mMeshes[nd->mMeshes[n]];

        apply_material(m_scene->mMaterials[mesh->mMaterialIndex]);

        if(mesh->mNormals == NULL) {
            glDisable(GL_LIGHTING);
        } else {
            glEnable(GL_LIGHTING);
        }

        for (t = 0; t < mesh->mNumFaces; ++t)
        {
            const struct aiFace* face = &mesh->mFaces[t];
            GLenum face_mode;

            switch(face->mNumIndices)
            {
                case 1: face_mode = GL_POINTS; break;
                case 2: face_mode = GL_LINES; break;
                case 3: face_mode = GL_TRIANGLES; break;
                default: face_mode = GL_POLYGON; break;
            }

            glBegin(face_mode);

            for(i = 0; i < face->mNumIndices; i++)
            {
                int index = face->mIndices[i];
                if(mesh->mColors[0] != NULL)
                    glColor4fv((GLfloat*)&mesh->mColors[0][index]);

                if(mesh->mNormals != NULL)
                    glNormal3fv(&mesh->mNormals[index].x);

                glVertex3fv(&mesh->mVertices[index].x);
            }

            glEnd();
        }

    }

    // draw all children
    for (n = 0; n < nd->mNumChildren; ++n)
        draw(nd->mChildren[n]);

    glPopMatrix();
}

