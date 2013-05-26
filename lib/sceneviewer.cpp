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
    //m_renderer = new GLRenderer();

    m_scaleFactor = 0.0f;

    m_cameraX = 0;
    m_cameraY = 0;
    m_cameraZ = 2.0f;

    m_angle = 0.f;
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
        SceneNode *node = new SceneNode();
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
    //m_cameraZ += event->delta() / 100;
    repaint();
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
    mainBufferSet->EnableBloom = true;
    mainBufferSet->SizeX = width();
    mainBufferSet->SizeY = height();
    mainBufferSet->Initialize();
}

void SceneViewer::resizeGL(int width, int height)
{
    //qDebug() << Q_FUNC_INFO;
    //Q_UNUSED(width)
    //Q_UNUSED(height)
/*
    const double aspectRatio = (float) width / height, fieldOfView = 45.0;
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(fieldOfView, aspectRatio, 0.0, 2000.0);  // Znear and Zfar
*/
    glViewport(0, 0, width, height);
    mainBufferSet->SizeX = width;
    mainBufferSet->SizeY = height;
}

void SceneViewer::paintGL()
{
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glDepthMask(GL_TRUE);
    glDepthFunc(GL_LEQUAL);
}

