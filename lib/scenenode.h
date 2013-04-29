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
#include <vector>

#include "GL/glew.h"
#include "GL/glut.h"

#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "math_3d.h"

#define INVALID_OGL_VALUE 0xFFFFFFFF
#define SAFE_DELETE(p) if (p) { delete p; p = NULL; }

struct Vertex
{
    Vector3f m_pos;
    Vector2f m_tex;
    Vector3f m_normal;

    Vertex() {}

    Vertex(const Vector3f& pos, const Vector2f& tex, const Vector3f& normal)
    {
        m_pos    = pos;
        m_tex    = tex;
        m_normal = normal;
    }
};

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

    float getScale();

    aiVector3D getSceneCenter();

    void render();

private:
    bool initFromScene(const aiScene* pScene, const std::string& Filename);
    void initMesh(unsigned int Index, const aiMesh* paiMesh);
    bool initMaterials(const aiScene* pScene, const std::string& Filename);
    void clear();

#define INVALID_MATERIAL 0xFFFFFFFF

    struct MeshEntry {
        MeshEntry();

        ~MeshEntry();

        void Init(const std::vector<Vertex>& Vertices,
                  const std::vector<unsigned int>& Indices);

        GLuint VB;
        GLuint IB;
        unsigned int NumIndices;
        unsigned int MaterialIndex;
    };

    std::vector<MeshEntry> m_Entries;
    //std::vector<Texture*> m_Textures;

    aiVector3D m_sceneMin, m_sceneMax;
    aiVector3D m_sceneCenter;

signals:
    void changed(quint32 id);
    
public slots:
    
};

#endif // SCENENODE_H
