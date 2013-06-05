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
#include "viewport.h"

#include <QDebug>

#define aisgl_min(x,y) (x<y?x:y)
#define aisgl_max(x,y) (y>x?y:x)

#define POSITIONS_VB  0
#define NORMALS_VB    1
#define TEXTURES_VB   2
#define TANGENTS_VB   3
#define BINORMALS_VB  4
#define INDICES_VB    5

#define ZERO_MEM(a) memset(a, 0, sizeof(a))
#define ARRAY_SIZE_IN_ELEMENTS(a) (sizeof(a)/sizeof(a[0]))
#define GLCheckError() (glGetError() == GL_NO_ERROR)

SceneNode::SceneNode(QObject *parent)
    : QObject(parent)
{
    m_useVertexArrays = true;
    m_VAO = 0;
    ZERO_MEM(m_VBO);
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
    if (m_VBO[0] != 0)
        glDeleteBuffers(ARRAY_SIZE_IN_ELEMENTS(m_VBO), m_VBO);

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
    glGenBuffers(ARRAY_SIZE_IN_ELEMENTS(m_VBO), m_VBO);

    bool Ret = false;

    const aiScene* pScene = aiImportFile(path.toStdString().c_str(), aiProcess_Triangulate | aiProcess_GenNormals | aiProcess_FlipUVs);

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

bool SceneNode::attachMaterial(MaterialData* material)
{
    m_material = material;

    return true;
}

float SceneNode::getScale()
{
    float scale = m_sceneMax.x - m_sceneMin.x;
    scale = aisgl_max(m_sceneMax.y - m_sceneMin.y, scale);
    scale = aisgl_max(m_sceneMax.z - m_sceneMin.z, scale);
    scale = 1.f / scale;

    return scale;
}

void SceneNode::setXposition(float x)
{
}

void SceneNode::setYposition(float y)
{
}

void SceneNode::setZposition(float z)
{
}

float SceneNode::Xposition()
{
    return 0.0; // TODO: Return the correct node position
}

float SceneNode::Yposition()
{
    return 0.0; // TODO: Return the correct node position
}

float SceneNode::Zposition()
{
    return 0.0; // TODO: Return the correct node position
}

aiVector3D SceneNode::getSceneCenter()
{
    return m_sceneMin;
}

void SceneNode::bind()
{
    if(m_useVertexArrays)
    {
        glBindVertexArray(m_VAO);
    }
    else
    {
        glBindBuffer(GL_ARRAY_BUFFER, m_VBO[POSITIONS_VB]);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
        glEnableVertexAttribArray(AttrPosition);

        glBindBuffer(GL_ARRAY_BUFFER, m_VBO[NORMALS_VB]);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
        glEnableVertexAttribArray(AttrNormal);

        glBindBuffer(GL_ARRAY_BUFFER, m_VBO[TEXTURES_VB]);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0);
        glEnableVertexAttribArray(AttrTexCoord);

        glBindBuffer(GL_ARRAY_BUFFER, m_VBO[TANGENTS_VB]);
        glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 0, 0);
        glEnableVertexAttribArray(AttrTangent);

        glBindBuffer(GL_ARRAY_BUFFER, m_VBO[BINORMALS_VB]);
        glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 0, 0);
        glEnableVertexAttribArray(AttrBiNormal);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_VBO[INDICES_VB]);
    }
}

bool SceneNode::initFromScene(const aiScene* pScene, const std::string& Filename)
{
    m_Entries.resize(pScene->mNumMeshes);
    //m_Textures.resize(pScene->mNumMaterials);

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
    m_Positions.reserve(NumVertices);
    m_Normals.reserve(NumVertices);
    m_TexCoords.reserve(NumVertices);
    m_Tangents.reserve(NumVertices);
    m_BiNormals.reserve(NumVertices);
    m_Indices.reserve(NumIndices);

    // Initialize the meshes in the scene one by one
    for (unsigned int i = 0 ; i < m_Entries.size() ; i++)
    {
        const aiMesh* paiMesh = pScene->mMeshes[i];
        initMesh(paiMesh);
    }

    //if (!InitMaterials(pScene, Filename))
    //    return false;

    // Position data
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO[POSITIONS_VB]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(m_Positions[0]) * m_Positions.size(), &m_Positions[0], GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(AttrPosition);

    qDebug() << "Normal Data Debug Info";
    for(uint i = 0; i < m_Normals.size() && i < 16; i++)
    {
        Vector3f blabla = m_Normals[i];
        qDebug() << blabla.x << blabla.x << blabla.x;
    }

    // Normal data
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO[NORMALS_VB]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(m_Normals[0]) * m_Normals.size(), &m_Normals[0], GL_STATIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(AttrNormal);

    // Texture data
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO[TEXTURES_VB]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(m_TexCoords[0]) * m_TexCoords.size(), &m_TexCoords[0], GL_STATIC_DRAW);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(AttrTexCoord);

    // Tangent data
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO[TANGENTS_VB]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(m_Tangents[0]) * m_Tangents.size(), &m_Tangents[0], GL_STATIC_DRAW);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(AttrTangent);

    // BiNormal data
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO[BINORMALS_VB]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(m_BiNormals[0]) * m_BiNormals.size(), &m_BiNormals[0], GL_STATIC_DRAW);
    glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(AttrBiNormal);

    // Index data
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_VBO[INDICES_VB]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(m_Indices[0]) * m_Indices.size(), &m_Indices[0], GL_STATIC_DRAW);

    return GLCheckError();
}

void SceneNode::initMesh(const aiMesh* paiMesh)
{
    const aiVector3D Zero3D(0.0f, 0.0f, 0.0f);

    // Populate the vertex attribute vectors
    for (unsigned int i = 0 ; i < paiMesh->mNumVertices ; i++)
    {
        const aiVector3D* pPos      = &(paiMesh->mVertices[i]);
        const aiVector3D* pNormal   = &(paiMesh->mNormals[i]);
        const aiVector3D* pTexCoord = paiMesh->HasTextureCoords(0) ? &(paiMesh->mTextureCoords[0][i]) : &Zero3D;

        m_Positions.push_back(Vector3f(pPos->x, pPos->y, pPos->z));
        m_Normals.push_back(Vector3f(pNormal->x, pNormal->y, pNormal->z));
        m_TexCoords.push_back(Vector2f(pTexCoord->x, pTexCoord->y));
    }

    // Populate the index buffer
    for (unsigned int i = 0 ; i < paiMesh->mNumFaces ; i++)
    {
        const aiFace& Face = paiMesh->mFaces[i];
        Q_ASSERT(Face.mNumIndices == 3);
        m_Indices.push_back(Face.mIndices[0]);
        m_Indices.push_back(Face.mIndices[1]);
        m_Indices.push_back(Face.mIndices[2]);
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

void SceneNode::generateTangents()
{
    // Info about current triangle
    Vector3f vPos[3];
    Vector2f vTex[3];
    int vIndices[3];
    int curIndex;

    // Vectors of 2 triangle sides in local and texture space
    Vector3f localA, localB;
    Vector2f texA, texB;

    // Vector multiplicators
    float rTan, sTan;
    float rBi, sBi;

    // Result vectors
    Vector3f Tangent, BiNormal;

    //Clean Up
    for (uint i = 0; i < m_Positions.size(); i++)
    {
        m_Tangents[i] = Vector3f(0.0f, 0.0f, 0.0f);
        m_BiNormals[i] = Vector3f(0.0f, 0.0f, 0.0f);
    }

    //Generation
    for (uint i = 0; i < m_Indices.size(); i+=3)
    {
        for (int j = 0; j < 3; j++)
        {
            curIndex = m_Indices[i + j];
            vIndices[j] = curIndex;

            vPos[j] = m_Positions[curIndex];
            vTex[j] = m_TexCoords[curIndex];
        }

        // Calculate local space Vectors
        localA = vPos[1] - vPos[0];
        localB = vPos[2] - vPos[0];

        // Calculate world space Vectors
        texA = vTex[1] - vTex[0];
        texB = vTex[2] - vTex[0];

        // Calculate Multiplicators
        // For tangent
        sTan = 1.0f / (texB.x - texA.x * texB.y / texA.y);
        rTan = 1.0f / (texA.x - texB.x * texA.y / texB.y);
        // For binormal
        sBi = 1.0f / (texB.y - texA.y * texB.x / texA.x);
        rBi = 1.0f / (texA.y - texB.y * texA.x / texB.x);

        // Calculate tangent and save to buffer
        Vector3f tmpTang = Vector3f(localA.x * rTan, localA.y * rTan, localA.z * rTan) +
                           Vector3f(localB.x * sTan, localB.y * sTan, localB.z * sTan);
        Tangent = tmpTang.Normalize(); //normalize(rTan * localA + sTan * localB);
        for (int j = 0; j < 3; j++)
            m_Tangents[vIndices[j]] += Vector3f(Tangent.x, Tangent.y, Tangent.z);

        // Calculate binormal and save to buffer
        Vector3f tmpBiNorm = Vector3f(localA.x * rBi, localA.y * rBi, localA.z * rBi) +
                             Vector3f(localB.x * sBi, localB.y * sBi, localB.z * sBi);
        BiNormal = tmpBiNorm.Normalize(); //normalize(rBi * localA + sBi * localB);
        for (int j = 0; j < 3; j++)
            m_BiNormals[vIndices[j]] += Vector3f(BiNormal.x, BiNormal.y, BiNormal.z);
    }
    // Normalizing Result
    Vector3f TmpVec;
    for (uint i = 0; i < m_Positions.size(); i++)
    {
        //Normalize Tangent
        TmpVec = Vector3f(m_Tangents[i]);
        TmpVec.Normalize();
        m_Tangents[i] = Vector3f(TmpVec.x, TmpVec.y, TmpVec.z);

        //Normalize BiNormal
        TmpVec = Vector3f(m_BiNormals[i]);
        TmpVec.Normalize();
        m_BiNormals[i] = Vector3f(TmpVec.x, TmpVec.y, TmpVec.z);
    }
}

void SceneNode::render()
{
    render(DrawingPassSolid);
}

void SceneNode::render(enum DrawingPass pass)
{
//    if(pass == DrawingPassSolidForced) // || curViewPort->FrustumCheck(Position, Mesh->maxRadius))
//    {
        if(!m_material->bind(pass))
            return;

        bind();

        //ShaderData::UniformMatrix4fv(MatModelView, m_modelMatrix);
        ViewPort::insertViewProjectionMatrix();
        //ShaderData::UniformMatrix4fv(MatViewProjection, curViewPort->ViewProjectionMatrix);
        ShaderData::ParseUniformInserts(m_uniformInserts);

        glDrawElements(GL_TRIANGLES, m_Positions.size(), GL_UNSIGNED_INT, 0);
//    }
}


