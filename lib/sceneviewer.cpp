/*
  Q Light Controller Plus
  sceneviewer.cpp

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

#include <QDebug>
#include <QMessageBox>
#include <QtGui>

SceneViewer::SceneViewer(QWidget *parent)
    : QGLWidget(parent)
{
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);

    m_textureUnitCount = 0;
}

SceneViewer::~SceneViewer()
{
}

quint32 SceneViewer::createNodeId()
{
    while (m_nodes.contains(m_latestNodeId) == true ||
           m_latestNodeId == SceneNode::invalidId())
    {
        m_latestNodeId++;
    }

    return m_latestNodeId;
}

quint32 SceneViewer::addNode(QString path, quint32 id)
{
    qDebug() << Q_FUNC_INFO << "Loading " << path << " into scene...";
    if (id == SceneNode::invalidId())
        id = createNodeId();

    if (m_nodes.contains(id) == true || id == SceneNode::invalidId())
    {
        qWarning() << Q_FUNC_INFO << "a node with ID" << id << "already exists!";
        return SceneNode::invalidId();
    }
    else
    {
        SceneNode *node = new SceneNode(this);
        // Load the model from the given path
        if (node->loadModel(path) == false)
        {
            delete node;
            return SceneNode::invalidId();
        }
        node->setShader(m_sceneShader);

        m_nodes[id] = node;
        node->setID(id);
        connect(node, SIGNAL(changed(quint32)),
                this, SIGNAL(nodeChanged(quint32)));

        return id;
    }
}

bool SceneViewer::deleteNode(quint32 id)
{
    if (m_nodes.contains(id) == true)
    {
        SceneNode* sn = m_nodes.take(id);
        Q_ASSERT(sn != NULL);

        delete sn;
        return true;
    }
    else
    {
        qWarning() << Q_FUNC_INFO << "No node with id" << id;
        return false;
    }
}

SceneNode* SceneViewer::node(quint32 id) const
{
    if (m_nodes.contains(id) == true)
        return m_nodes[id];
    else
        return NULL;
}

QList <SceneNode*> SceneViewer::nodes() const
{
    return m_nodes.values();
}

void SceneViewer::wheelEvent(QWheelEvent *event)
{
    m_view.setZ(m_view.getZ() + event->delta() / 100);
    repaint();
}

int SceneViewer::getTextureUnitCount()
{
    return m_textureUnitCount;
}

void SceneViewer::increaseTextureUnitCount()
{
    m_textureUnitCount++;
}

BufferSet *SceneViewer::getBufferSet()
{
    return mainBufferSet;
}

void SceneViewer::loadShaders()
{
    m_normalsShader = ShaderData::FromPlainText("normal");
    m_sceneShader = ShaderData::FromPlainText("scenepass");
    m_planeShader = ShaderData::FromPlainText("plane");
}

// ********************************* OPENGL reimplemented methods *********************************

void SceneViewer::initializeGL()
{
    qDebug() << Q_FUNC_INFO;

    int argc = 0;
    glutInit(&argc, NULL);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);

    qDebug() << "Vendor: " << QString((const char *)glGetString (GL_VENDOR));
    qDebug() << "Renderer: " << QString((const char *)glGetString (GL_RENDERER));
    qDebug() << "Version: " << QString((const char *)glGetString (GL_VERSION));
    qDebug() << "GLSL: " << QString((const char *)glGetString (GL_SHADING_LANGUAGE_VERSION));

    //glewExperimental = GL_TRUE;
    GLenum res = glewInit();
    if (res != GLEW_OK)
    {
        qDebug() <<  "Error: " << glewGetErrorString(res);
        return;
    }

    // Initialize Renderbuffers
    mainBufferSet = new BufferSet();
    mainBufferSet->EnableSsao = true;
    mainBufferSet->EnableBloom = false;
    mainBufferSet->m_width = width();
    mainBufferSet->m_height = height();
    mainBufferSet->initialize();

    // load shaders
    loadShaders();

    // create scene filters
    m_normalsFilter = new SceneFilter(this);
    m_normalsFilter->setShader(m_normalsShader);
    m_normalsFilter->addFBTexture(TexNormal, "NormalColor");

    m_planeFilter = new SceneFilter(this);
    m_planeFilter->setShader(m_planeShader);
    m_planeFilter->addFBTexture(TexDiffuse, "Scene");

    // Create 2D filter
    m_filter2D = new Filter2D(this);

    // Setup View
    m_view.bind();
}

void SceneViewer::resizeGL(int width, int height)
{
    //qDebug() << Q_FUNC_INFO;
    glViewport(0, 0, width, height);
    mainBufferSet->m_width = width;
    mainBufferSet->m_height = height;
    mainBufferSet->OutBuffer->setSize(width, height);
    mainBufferSet->initialize();
}

void SceneViewer::paintGL()
{

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glDepthMask(GL_TRUE);
    glDepthFunc(GL_LEQUAL);

    m_textureUnitCount = 0;

    //Draw normals pass
    mainBufferSet->NormalPass.Bind(true);
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_BLEND);
    foreach(SceneNode *sn, nodes())
    {
        sn->setShader(m_normalsShader);
        sn->render();
    }
    m_filter2D->Draw(m_normalsFilter);

    mainBufferSet->ScenePass.Bind(true);
    foreach(SceneNode *sn, nodes())
    {
        sn->setShader(m_sceneShader);
        sn->render();
    }
    mainBufferSet->OutBuffer->Bind(false);
    m_filter2D->Draw(m_planeFilter);
}

