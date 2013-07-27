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

#include <QDebug>
#include <QMessageBox>
#include <QtGui>

#include "sceneviewer.h"

SceneViewer::SceneViewer(QWidget *parent)
    : QGLWidget(parent)
{
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);

    m_modelsShader = NULL;
    m_viewPort = NULL;
    m_scale = 0.0f;
    m_debugBuffers = false;
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
        SceneNode *node = new SceneNode(m_viewPort);
        if (m_modelsShader != NULL)
            node->setShader(m_modelsShader);
        // Load the model from the given path
        if (node->loadModel(path) == false)
        {
            delete node;
            return SceneNode::invalidId();
        }

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
    Q_UNUSED(event)
    m_viewPort->setZ(m_viewPort->getZ() + event->delta() / 100);
    repaint();
}


// ********************************* OPENGL reimplemented methods *********************************

void SceneViewer::initializeGL()
{
    qDebug() << Q_FUNC_INFO;

    //int argc = 0;
    //glutInit(&argc, NULL);
    //glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);

    qDebug() << "Vendor: " << QString((const char *)glGetString (GL_VENDOR));
    qDebug() << "Renderer: " << QString((const char *)glGetString (GL_RENDERER));
    qDebug() << "Version: " << QString((const char *)glGetString (GL_VERSION));
    qDebug() << "GLSL: " << QString((const char *)glGetString (GL_SHADING_LANGUAGE_VERSION));

    glDisable(GL_LIGHTING);
    glEnable(GL_TEXTURE_2D);
    glShadeModel(GL_SMOOTH);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    //glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

    //glewExperimental = GL_TRUE;
    GLenum err = glewInit();
    if (GLEW_OK != err)
        return;

    m_width = width();
    m_height = height();

    m_modelsShader = new Shader("shaders/deferredShading.vert", "shaders/deferredShading.frag");

    m_viewPort = new ViewPort();
    m_multipleRenderTarget = new FBORenderTexture(m_width, m_height);
    m_deferredRendering = new DeferredRendering(m_width, m_height, m_multipleRenderTarget, m_viewPort);
}

void SceneViewer::resizeGL(int width, int height)
{
    //qDebug() << Q_FUNC_INFO;
    m_width = width;
    m_height = height;
    m_multipleRenderTarget->setSize(m_width, m_height);

    glViewport(0, 0, m_width, m_height);
}

void SceneViewer::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    //glClearColor(0.2f, 0.3f, 0.8f, 1.0f);

    // Render our geometry into the FBO
    m_multipleRenderTarget->start();
    foreach(SceneNode *sn, nodes())
        sn->render();
    m_multipleRenderTarget->stop();

    // Render to the screen
    if(m_debugBuffers == false)
    {
        // Render to screen using the deferred rendering shader
        m_deferredRendering->render();
    }
    else
    {
        int halfWidth = width() / 2;
        int halfHeight = height() / 2;
        m_multipleRenderTarget->showTexture( 0, halfWidth, halfHeight, 0);
        m_multipleRenderTarget->showTexture( 1, halfWidth, halfHeight, halfWidth);
        m_multipleRenderTarget->showTexture( 2, halfWidth, halfHeight, 0, halfHeight);
        //m_multipleRenderTarget->showTexture( 3, halfWidth, halfHeight, halfWidth, halfHeight);
    }
}

