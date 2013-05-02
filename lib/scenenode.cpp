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

#define POSITION_LOCATION 0
#define TEX_COORD_LOCATION 1
#define NORMAL_LOCATION 2
#define WVP_LOCATION 3
#define WORLD_LOCATION 7

SceneNode::SceneNode(QObject *parent)
    : QObject(parent)
{
    m_VAO = 0;
    ZERO_MEM(m_Buffers);
}

SceneNode::~SceneNode()
{
    clear();
}

void SceneNode::clear()
{
/*
    for (unsigned int i = 0 ; i < m_Textures.size() ; i++) {
        SAFE_DELETE(m_Textures[i]);
    }
*/
    if (m_Buffers[0] != 0)
        glDeleteBuffers(ARRAY_SIZE_IN_ELEMENTS(m_Buffers), m_Buffers);

    if (m_VAO != 0)
    {
        glDeleteVertexArrays(1, &m_VAO);
        m_VAO = 0;
    }
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

bool SceneNode::loadModel(QString path)
{
    // Release the previously loaded mesh (if it exists)
    clear();

    // Create the VAO
    glGenVertexArrays(1, &m_VAO);
    glBindVertexArray(m_VAO);

    // Create the buffers for the vertices attributes
    glGenBuffers(ARRAY_SIZE_IN_ELEMENTS(m_Buffers), m_Buffers);

    bool Ret = false;

    const aiScene* pScene = aiImportFile(path.toStdString().c_str(), aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs);

    if (pScene)
    {
        Ret = initFromScene(pScene, path.toStdString().c_str());
    }
    else
    {
        qDebug() << Q_FUNC_INFO << "Cannot load " << path << " !!";
        Ret = false;
    }

    // Make sure the VAO is not changed from the outside
    glBindVertexArray(0);

    return Ret;
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

bool SceneNode::initFromScene(const aiScene* pScene, const std::string& Filename)
{
    m_Entries.resize(pScene->mNumMeshes);
    //m_Textures.resize(pScene->mNumMaterials);

    std::vector<Vector3f> Positions;
    std::vector<Vector3f> Normals;
    std::vector<Vector2f> TexCoords;
    std::vector<unsigned int> Indices;

    unsigned int NumVertices = 0;
    unsigned int NumIndices = 0;

    // Initialize the meshes in the scene one by one
    for (unsigned int i = 0 ; i < m_Entries.size() ; i++)
    {
        m_Entries[i].MaterialIndex = pScene->mMeshes[i]->mMaterialIndex;
        m_Entries[i].NumIndices = pScene->mMeshes[i]->mNumFaces * 3;
        m_Entries[i].BaseVertex = NumVertices;
        m_Entries[i].BaseIndex = NumIndices;

        NumVertices += pScene->mMeshes[i]->mNumVertices;
        NumIndices  += m_Entries[i].NumIndices;
    }

    // Reserve space in the vectors for the vertex attributes and indices
    Positions.reserve(NumVertices);
    Normals.reserve(NumVertices);
    TexCoords.reserve(NumVertices);
    Indices.reserve(NumIndices);

    // Initialize the meshes in the scene one by one
    for (unsigned int i = 0 ; i < m_Entries.size() ; i++)
    {
        const aiMesh* paiMesh = pScene->mMeshes[i];
        initMesh(paiMesh, Positions, Normals, TexCoords, Indices);
    }

    //if (!InitMaterials(pScene, Filename))
    //    return false;

    // Generate and populate the buffers with vertex attributes and the indices
    glBindBuffer(GL_ARRAY_BUFFER, m_Buffers[POS_VB]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Positions[0]) * Positions.size(), &Positions[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(POSITION_LOCATION);
    glVertexAttribPointer(POSITION_LOCATION, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glBindBuffer(GL_ARRAY_BUFFER, m_Buffers[TEXCOORD_VB]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(TexCoords[0]) * TexCoords.size(), &TexCoords[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(TEX_COORD_LOCATION);
    glVertexAttribPointer(TEX_COORD_LOCATION, 2, GL_FLOAT, GL_FALSE, 0, 0);

    glBindBuffer(GL_ARRAY_BUFFER, m_Buffers[NORMAL_VB]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Normals[0]) * Normals.size(), &Normals[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(NORMAL_LOCATION);
    glVertexAttribPointer(NORMAL_LOCATION, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Buffers[INDEX_BUFFER]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Indices[0]) * Indices.size(), &Indices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, m_Buffers[WVP_MAT_VB]);

    for (unsigned int i = 0; i < 4 ; i++)
    {
        glEnableVertexAttribArray(WVP_LOCATION + i);
        glVertexAttribPointer(WVP_LOCATION + i, 4, GL_FLOAT, GL_FALSE, sizeof(Matrix4f), (const GLvoid*)(sizeof(GLfloat) * i * 4));
        glVertexAttribDivisor(WVP_LOCATION + i, 1);
    }

    glBindBuffer(GL_ARRAY_BUFFER, m_Buffers[WORLD_MAT_VB]);

    for (unsigned int i = 0; i < 4 ; i++)
    {
        glEnableVertexAttribArray(WORLD_LOCATION + i);
        glVertexAttribPointer(WORLD_LOCATION + i, 4, GL_FLOAT, GL_FALSE, sizeof(Matrix4f), (const GLvoid*)(sizeof(GLfloat) * i * 4));
        glVertexAttribDivisor(WORLD_LOCATION + i, 1);
    }

    return GLCheckError();
}

void SceneNode::initMesh(const aiMesh* paiMesh,
                    std::vector<Vector3f>& Positions,
                    std::vector<Vector3f>& Normals,
                    std::vector<Vector2f>& TexCoords,
                    std::vector<unsigned int>& Indices)
{
    const aiVector3D Zero3D(0.0f, 0.0f, 0.0f);

    // Populate the vertex attribute vectors
    for (unsigned int i = 0 ; i < paiMesh->mNumVertices ; i++)
    {
        const aiVector3D* pPos      = &(paiMesh->mVertices[i]);
        const aiVector3D* pNormal   = &(paiMesh->mNormals[i]);
        const aiVector3D* pTexCoord = paiMesh->HasTextureCoords(0) ? &(paiMesh->mTextureCoords[0][i]) : &Zero3D;

        Positions.push_back(Vector3f(pPos->x, pPos->y, pPos->z));
        Normals.push_back(Vector3f(pNormal->x, pNormal->y, pNormal->z));
        TexCoords.push_back(Vector2f(pTexCoord->x, pTexCoord->y));
    }

    // Populate the index buffer
    for (unsigned int i = 0 ; i < paiMesh->mNumFaces ; i++)
    {
        const aiFace& Face = paiMesh->mFaces[i];
        Q_ASSERT(Face.mNumIndices == 3);
        Indices.push_back(Face.mIndices[0]);
        Indices.push_back(Face.mIndices[1]);
        Indices.push_back(Face.mIndices[2]);
    }
}
/*
bool SceneNode::initMaterials(const aiScene* pScene, const std::string& Filename)
{
    // Extract the directory part from the file name
    std::string::size_type SlashIndex = Filename.find_last_of("/");
    std::string Dir;

    if (SlashIndex == std::string::npos) {
        Dir = ".";
    }
    else if (SlashIndex == 0) {
        Dir = "/";
    }
    else {
        Dir = Filename.substr(0, SlashIndex);
    }

    bool Ret = true;

    // Initialize the materials
    for (unsigned int i = 0 ; i < pScene->mNumMaterials ; i++)
    {
        const aiMaterial* pMaterial = pScene->mMaterials[i];

        m_Textures[i] = NULL;

        if (pMaterial->GetTextureCount(aiTextureType_DIFFUSE) > 0) {
            aiString Path;

            if (pMaterial->GetTexture(aiTextureType_DIFFUSE, 0, &Path, NULL, NULL, NULL, NULL, NULL) == AI_SUCCESS) {
                std::string FullPath = Dir + "/" + Path.data;
                m_Textures[i] = new Texture(GL_TEXTURE_2D, FullPath.c_str());

                if (!m_Textures[i]->Load()) {
                    printf("Error loading texture '%s'\n", FullPath.c_str());
                    delete m_Textures[i];
                    m_Textures[i] = NULL;
                    Ret = false;
                }
                else {
                    printf("Loaded texture '%s'\n", FullPath.c_str());
                }
            }
        }

        // Load a white texture in case the model does not include its own texture
        if (!m_Textures[i])
        {
            m_Textures[i] = new Texture(GL_TEXTURE_2D, "./white.png");

            Ret = m_Textures[i]->Load();
        }
    }

    return Ret;
}
*/
void SceneNode::render()
{
    glBindVertexArray(m_VAO);

    for (unsigned int i = 0 ; i < m_Entries.size() ; i++)
    {
/*
        const unsigned int MaterialIndex = m_Entries[i].MaterialIndex;
        assert(MaterialIndex < m_Textures.size());

        if (m_Textures[MaterialIndex])
            m_Textures[MaterialIndex]->Bind(GL_TEXTURE0);
*/
        glDrawElementsBaseVertex(GL_TRIANGLES,
                                 m_Entries[i].NumIndices,
                                 GL_UNSIGNED_INT,
                                 (void*)(sizeof(unsigned int) * m_Entries[i].BaseIndex),
                                 m_Entries[i].BaseVertex);
    }

    // Make sure the VAO is not changed from the outside
    glBindVertexArray(0);
}

void SceneNode::render(unsigned int NumInstances, const Matrix4f* WVPMats, const Matrix4f* WorldMats)
{
    glBindBuffer(GL_ARRAY_BUFFER, m_Buffers[WVP_MAT_VB]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Matrix4f) * NumInstances, WVPMats, GL_DYNAMIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, m_Buffers[WORLD_MAT_VB]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Matrix4f) * NumInstances, WorldMats, GL_DYNAMIC_DRAW);

    glBindVertexArray(m_VAO);

    for (unsigned int i = 0 ; i < m_Entries.size() ; i++)
    {
/*
        const unsigned int MaterialIndex = m_Entries[i].MaterialIndex;
        assert(MaterialIndex < m_Textures.size());

        if (m_Textures[MaterialIndex])
            m_Textures[MaterialIndex]->Bind(GL_TEXTURE0);
*/
        glDrawElementsInstancedBaseVertex(GL_TRIANGLES,
                                          m_Entries[i].NumIndices,
                                          GL_UNSIGNED_INT,
                                          (void*)(sizeof(unsigned int) * m_Entries[i].BaseIndex),
                                          NumInstances,
                                          m_Entries[i].BaseVertex);
    }

    // Make sure the VAO is not changed from the outside
    glBindVertexArray(0);
}


