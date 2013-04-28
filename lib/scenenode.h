/*
  Q Light Controller Plus
  scenenode.h

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

#ifndef SCENENODE_H
#define SCENENODE_H

#include <QObject>

#include "GL/glew.h"
#include "GL/glut.h"

#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

class SceneNode : public QObject
{
    Q_OBJECT
public:
    SceneNode(QObject *parent = 0);

    /*****************************************************************************
     * Node ID
     *****************************************************************************/
    /**
     * Change the node instance ID
     *
     * @param id The new node id
     */
    void setID(quint32 id);

    /**
     * Get the node instance ID.
     *
     * @return The node ID
     */
    quint32 id() const;

    /**
     * Get the invalid node ID (for comparison etc...)
     */
    static quint32 invalidId();

protected:
    /** Unique ID of the node */
    quint32 m_id;

    /*****************************************************************************
     * Node model
     *****************************************************************************/
public:
    bool loadModel(QString path);

    const aiScene *getModel() const;

    float getScale();

    aiVector3D getSceneCenter();

    void apply_material(const struct aiMaterial *mtl);
    void draw (const struct aiNode* nd = NULL);

protected:
    void getBoundingBoxForNode (const aiNode* nd, aiVector3D* min, aiVector3D* max, aiMatrix4x4* trafo);
    void getBoundingBox (aiVector3D* min, aiVector3D* max);

    /** Reference to the AssImp object for rendering and transformations */
    const aiScene* m_scene;

    aiVector3D m_sceneMin, m_sceneMax;
    aiVector3D m_sceneCenter;

signals:
    void changed(quint32 id);
    
public slots:
    
};

#endif // SCENENODE_H
