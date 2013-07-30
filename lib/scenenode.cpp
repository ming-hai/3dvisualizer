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

#include "sceneviewer.h"
#include "scenenode.h"

#include <QDebug>
#include <QDir>

#define aisgl_min(x,y) (x<y?x:y)
#define aisgl_max(x,y) (y>x?y:x)

#define POSITION_LOCATION           0
#define NORMAL_LOCATION             1
#define TEX_COORD_LOCATION          2

SceneNode::SceneNode(ViewPort *view, QObject *parent)
    : QObject(parent)
    , m_view(view)
{
    m_VAO = 0;
    ZERO_MEM(m_VBO);
    m_nodePosition.x = 0.0;
    m_nodePosition.y = 0.0;
    m_nodePosition.z = 0.0;

    m_nodeRotation.x = 0.0;
    m_nodeRotation.y = 0.0;
    m_nodeRotation.z = 0.0;

    m_rotationMatrix.InitIdentity();
    m_translationMatrix.InitIdentity();

    m_modelBaseDirectory = "";

    m_shader = NULL;
}

SceneNode::~SceneNode()
{
    clear();
}

void SceneNode::clear()
{
    foreach(Texture *texture, m_textures)
        m_textures.removeOne(texture);

    if (m_VBO[0] != 0)
        glDeleteBuffers(NUM_VBOS, m_VBO);

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
    glGenBuffers(NUM_VBOS, m_VBO);

    bool Ret = false;

    const aiScene* pScene = aiImportFile(path.toStdString().c_str(), aiProcess_Triangulate | aiProcess_GenNormals | aiProcess_FlipUVs);

    if (pScene)
    {
        QFileInfo info(path);
        m_modelBaseDirectory = info.absoluteFilePath();
        Ret = initFromScene(pScene);
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

bool SceneNode::loadModelFromBuffer(QString buffer)
{
    // Release the previously loaded mesh (if it exists)
    clear();

    // Create the VAO
    glGenVertexArrays(1, &m_VAO);
    glBindVertexArray(m_VAO);

    // Create the buffers for the vertices attributes
    glGenBuffers(NUM_VBOS, m_VBO);

    bool Ret = false;

    const aiScene* pScene = aiImportFileFromMemory(buffer.toAscii().data(), buffer.length(),
                                                   aiProcess_Triangulate | aiProcess_GenNormals | aiProcess_FlipUVs, NULL);

    if (pScene)
    {
        Ret = initFromScene(pScene);
    }
    else
    {
        qDebug() << Q_FUNC_INFO << "Cannot load model from buffer !!";
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

void SceneNode::setXposition(float x)
{
    m_nodePosition.x = x;
    m_translationMatrix.InitTranslationTransform(m_nodePosition.x, m_nodePosition.y, m_nodePosition.z);
}

void SceneNode::setYposition(float y)
{
    m_nodePosition.y = y;
    m_translationMatrix.InitTranslationTransform(m_nodePosition.x, m_nodePosition.y, m_nodePosition.z);
}

void SceneNode::setZposition(float z)
{
    m_nodePosition.z = z;
    m_translationMatrix.InitTranslationTransform(m_nodePosition.x, m_nodePosition.y, m_nodePosition.z);
}

float SceneNode::Xposition()
{
    return m_nodePosition.x;
}

float SceneNode::Yposition()
{
    return m_nodePosition.y;
}

float SceneNode::Zposition()
{
    return m_nodePosition.z;
}

void SceneNode::setXrotation(float x)
{
    m_nodeRotation.x = x;
    m_rotationMatrix.InitRotateTransform(m_nodeRotation.x, m_nodeRotation.y, m_nodeRotation.z);
}

void SceneNode::setYrotation(float y)
{
    m_nodeRotation.y = y;
    m_rotationMatrix.InitRotateTransform(m_nodeRotation.x, m_nodeRotation.y, m_nodeRotation.z);
}

void SceneNode::setZrotation(float z)
{
    m_nodeRotation.z = z;
    m_rotationMatrix.InitRotateTransform(m_nodeRotation.x, m_nodeRotation.y, m_nodeRotation.z);
}

float SceneNode::Xrotation()
{
    return m_nodeRotation.x;
}

float SceneNode::Yrotation()
{
    return m_nodeRotation.y;
}

float SceneNode::Zrotation()
{
    return m_nodeRotation.z;
}

aiVector3D SceneNode::getSceneCenter()
{
    return m_sceneMin;
}

quint32 SceneNode::getSize()
{
    return m_Positions.size();
}

void SceneNode::setShader(Shader *sh)
{
    m_shader = sh;

    if (m_shader != NULL)
    {
        glUseProgram(m_shader->m_shaderProg);
        glBindAttribLocation(m_shader->m_shaderProg, POSITION_LOCATION, "in_Position");
        glBindAttribLocation(m_shader->m_shaderProg, NORMAL_LOCATION, "in_Normal");
        glBindAttribLocation(m_shader->m_shaderProg, TEX_COORD_LOCATION, "in_TexCoord");
        glLinkProgram(m_shader->m_shaderProg);

        m_modelMatrixLoc = glGetUniformLocation(m_shader->m_shaderProg, "ModelMatrix");
        m_worldMatrixLoc = glGetUniformLocation(m_shader->m_shaderProg, "WorldMatrix");
        m_textUniLoc = glGetUniformLocation(m_shader->m_shaderProg, "tDiffuse");
        glUseProgram(0);
    }
}

Shader *SceneNode::getShader()
{
    return m_shader;
}

bool SceneNode::initFromScene(const aiScene* pScene)
{
    m_Entries.resize(pScene->mNumMeshes);
    //m_textures.resize(pScene->mNumMaterials);

    qDebug() << "Number of entries found: " << pScene->mNumMeshes;

    unsigned int NumVertices = 0;
    unsigned int NumIndices = 0;

    // Initialize the meshes in the scene one by one
    for (unsigned int i = 0 ; i < m_Entries.size() ; i++)
    {
        //qDebug() << "Mesh name: " << QLatin1String(pScene->mMeshes[i]->mName.data);
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
    m_Indices.reserve(NumIndices);

    // Initialize the meshes in the scene one by one
    for (unsigned int i = 0 ; i < m_Entries.size() ; i++)
        initMesh(pScene->mMeshes[i]);

    initMaterials(pScene);

    // Position data
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO[POSITIONS_VB]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(m_Positions[0]) * m_Positions.size(), &m_Positions[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(POSITION_LOCATION);
    glVertexAttribPointer(POSITION_LOCATION, 3, GL_FLOAT, GL_FALSE, 0, 0);

    if (GLCheckError() == false)
    {
        qDebug() << "Error in binding position VBO !";
        return false;
    }

    // Normal data
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO[NORMALS_VB]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(m_Normals[0]) * m_Normals.size(), &m_Normals[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(NORMAL_LOCATION);
    glVertexAttribPointer(NORMAL_LOCATION, 3, GL_FLOAT, GL_FALSE, 0, 0);

    if (GLCheckError() == false)
    {
        qDebug() << "Error in binding normals VBO !";
        return false;
    }

    // Texture data
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO[TEXTURES_VB]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(m_TexCoords[0]) * m_TexCoords.size(), &m_TexCoords[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(TEX_COORD_LOCATION);
    glVertexAttribPointer(TEX_COORD_LOCATION, 2, GL_FLOAT, GL_FALSE, 0, 0);

    if (GLCheckError() == false)
    {
        qDebug() << "Error in binding textures VBO !";
        return false;
    }

    // Index data
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_VBO[INDICES_VB]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(m_Indices[0]) * m_Indices.size(), &m_Indices[0], GL_STATIC_DRAW);

    if (GLCheckError() == false)
    {
        qDebug() << "Error in binding indices VBO !";
        return false;
    }

    return true;
}

void SceneNode::initMesh(const aiMesh* paiMesh)
{
    const aiVector3D Zero3D(0.0f, 0.0f, 0.0f);

    //qDebug() << "initMesh vertices:" << paiMesh->mNumVertices;

    // Populate the vertex attribute vectors
    for (unsigned int i = 0 ; i < paiMesh->mNumVertices; i++)
    {
        const aiVector3D* pPos = &(paiMesh->mVertices[i]);
        if (pPos)
            m_Positions.push_back(Vector3f(pPos->x, pPos->y, pPos->z));

        const aiVector3D* pNormal = &(paiMesh->mNormals[i]);
        if (paiMesh->HasNormals())
            m_Normals.push_back(Vector3f(pNormal->x, pNormal->y, pNormal->z));

        const aiVector3D* pTexCoord = paiMesh->HasTextureCoords(0) ? &(paiMesh->mTextureCoords[0][i]) : &Zero3D;
        m_TexCoords.push_back(Vector2f(pTexCoord->x, pTexCoord->y));
    }

    // Populate the index buffer
    for (unsigned int i = 0 ; i < paiMesh->mNumFaces ; i++)
    {
        const aiFace& Face = paiMesh->mFaces[i];
        //qDebug() << "Number of faces: " << Face.mNumIndices;
        //Q_ASSERT(Face.mNumIndices == 3);
        if (Face.mNumIndices == 1)
        {
            m_Indices.push_back(Face.mIndices[0]);
        }
        else if (Face.mNumIndices == 2)
        {
            m_Indices.push_back(Face.mIndices[0]);
            m_Indices.push_back(Face.mIndices[1]);
        }
        else if (Face.mNumIndices == 3)
        {
            m_Indices.push_back(Face.mIndices[0]);
            m_Indices.push_back(Face.mIndices[1]);
            m_Indices.push_back(Face.mIndices[2]);
        }
    }
}

bool SceneNode::initMaterials(const aiScene* pScene)
{
    // Initialize the materials
    for (unsigned int i = 0 ; i < pScene->mNumMaterials ; i++)
    {
        const aiMaterial* pMaterial = pScene->mMaterials[i];
        bool ret = false;

        Texture* texture = new Texture(GL_TEXTURE_2D);
        aiColor4D clr;

        aiString aName;
        if (pMaterial->Get(AI_MATKEY_NAME, aName) == aiReturn_SUCCESS && aName.length > 0)
            qDebug() << "Material name:"  << QLatin1String(aName.data);

        if(pMaterial->Get(AI_MATKEY_COLOR_AMBIENT, clr) == aiReturn_SUCCESS)
        {
            ret = texture->loadMaterial(QColor::fromRgbF(clr.r, clr.g, clr.b, clr.a));
        }
        else if(pMaterial->Get(AI_MATKEY_COLOR_DIFFUSE, clr) == aiReturn_SUCCESS)
        {
            ret = texture->loadMaterial(QColor::fromRgbF(clr.r, clr.g, clr.b, clr.a));
        }
        else if (pMaterial->GetTextureCount(aiTextureType_DIFFUSE) > 0)
        {
            aiString Path;

            if (pMaterial->GetTexture(aiTextureType_DIFFUSE, 0, &Path, NULL, NULL, NULL, NULL, NULL) == AI_SUCCESS)
            {
                QString textPath = m_modelBaseDirectory + QDir::separator() + Path.data;
                ret = texture->loadTexture(textPath);
            }
        }
        if (ret == false)
        {
            delete texture;
            texture = NULL;
        }
        else
            m_textures.append(texture);
    }

    return true;
}

void SceneNode::render(bool applyMaterials)
{
    // bind the shader
    if (m_shader)
        glUseProgram(m_shader->m_shaderProg);

    glBindVertexArray(m_VAO);

    Matrix4f modelMatrix = m_rotationMatrix * m_translationMatrix;

    glUniformMatrix4fv ( m_modelMatrixLoc, 1, GL_TRUE, (const GLfloat*)modelMatrix.m_matrix);

    Matrix4f worldMatrix = m_view->projectionMatrix();
    glUniformMatrix4fv ( m_worldMatrixLoc, 1, GL_TRUE, (const GLfloat*)worldMatrix.m_matrix);

    for (unsigned int i = 0 ; i < m_Entries.size() ; i++)
    {
        if (applyMaterials)
        {
            const unsigned int MaterialIndex = m_Entries[i].MaterialIndex;
            Q_ASSERT(MaterialIndex < m_textures.size());

            if (m_textures[MaterialIndex])
            {
                m_textures[MaterialIndex]->bind(GL_TEXTURE0 + MaterialIndex);
                glUniform1i ( m_textUniLoc, MaterialIndex);
            }
        }

        glDrawElementsBaseVertex(GL_TRIANGLES,
                                 m_Entries[i].NumIndices,
                                 GL_UNSIGNED_INT,
                                 (void*)(sizeof(unsigned int) * m_Entries[i].BaseIndex),
                                 m_Entries[i].BaseVertex);
    }

    // Make sure the VAO is not changed from the outside
    glBindVertexArray(0);
    glUseProgram(0);
}

