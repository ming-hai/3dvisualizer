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
#include <QDir>

#include "GL/glew.h"
#include "GL/glut.h"

#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/material.h>
#include <assimp/postprocess.h>

#include "math_3d.h"
#include "shader.h"
#include "texture.h"
#include "viewport.h"

#define INVALID_MATERIAL 0xFFFFFFFF

#define ZERO_MEM(a) memset(a, 0, sizeof(a))
#define GLCheckError() (glGetError() == GL_NO_ERROR)
#define SAFE_DELETE(p) if (p) { delete p; p = NULL; }
#define ARRAY_SIZE_IN_ELEMENTS(a) (sizeof(a)/sizeof(a[0]))

#define MODELS_FOLDER "models"

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

enum ModelVBOs
{
    POSITIONS_VB,
    NORMALS_VB,
    TEXTURES_VB,
    INDICES_VB,
    NUM_VBOS
};

class SceneNode : public QObject
{
    Q_OBJECT
public:
    SceneNode(ViewPort *view, QObject *parent = 0);

    ~SceneNode();

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
    bool loadModelFromBuffer(QString buffer);

    float getScale();

    // Node position
    void setXposition(float x);
    void setYposition(float y);
    void setZposition(float z);

    float Xposition();
    float Yposition();
    float Zposition();

    // Node rotation
    void setXrotation(float x);
    void setYrotation(float y);
    void setZrotation(float z);

    float Xrotation();
    float Yrotation();
    float Zrotation();

    aiVector3D getSceneCenter();

    quint32 getSize();

    void setShader(Shader *sh);
    Shader *getShader();

    void render(bool applyMaterials = true);

protected:
    bool m_emitLight;

private:
    bool initFromScene(const aiScene* pScene);
    void initMesh(const aiMesh* paiMesh);
    bool initMaterials(const aiScene* pScene);
    void clear();

    GLuint m_VAO;
    GLuint m_VBO[NUM_VBOS];

    Matrix4f m_translationMatrix;
    Matrix4f m_rotationMatrix;

    std::vector<Vector3f> m_Positions;
    std::vector<Vector3f> m_Normals;
    std::vector<Vector2f> m_TexCoords;
    std::vector<unsigned int> m_Indices;

    struct MeshEntry
    {
        MeshEntry()
        {
            NumIndices = 0;
            BaseVertex = 0;
            BaseIndex = 0;
            MaterialIndex = INVALID_MATERIAL;
        }

        unsigned int NumIndices;
        unsigned int BaseVertex;
        unsigned int BaseIndex;
        unsigned int MaterialIndex;
    };

    std::vector<MeshEntry> m_Entries;
    QList<Texture*> m_textures;

    aiVector3D m_sceneMin, m_sceneMax;
    Vector3f m_nodePosition, m_nodeRotation;

    QString m_modelBaseDirectory;

    Shader *m_shader;
    ViewPort *m_view; // used for world matrix insertion

    GLuint m_modelMatrixLoc;
    GLuint m_worldMatrixLoc;
    GLuint m_textUniLoc;

signals:
    void changed(quint32 id);
    
public slots:
    
};

#endif // SCENENODE_H
