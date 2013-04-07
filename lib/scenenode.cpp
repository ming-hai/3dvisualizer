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

