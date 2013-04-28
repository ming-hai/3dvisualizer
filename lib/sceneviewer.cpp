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
    m_scene_list = 0;
    m_renderer = new GLRenderer();

    m_scaleFactor = 1.f;

    m_cameraX = 0;
    m_cameraY = 0;
    m_cameraZ = 2.0f;

    m_angle = 0.f;
    m_sceneList = 0;

    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_DEPTH_TEST);

    //glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
    glEnable(GL_NORMALIZE);
    //glColorMaterial(GL_FRONT_AND_BACK, GL_DIFFUSE);

    qDebug() << "Vendor: " << glGetString (GL_VENDOR);
    qDebug() << "Renderer: " << glGetString (GL_RENDERER);
    qDebug() << "Version: " << glGetString (GL_VERSION);
    qDebug() << "GLSL: " << glGetString (GL_SHADING_LANGUAGE_VERSION);
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

// ********************************* OPENGL reimplemented methods *********************************

void SceneViewer::initializeGL()
{
    //qDebug() << Q_FUNC_INFO;
}

void SceneViewer::resizeGL(int width, int height)
{
    //qDebug() << Q_FUNC_INFO;

    const double aspectRatio = (float) width / height, fieldOfView = 45.0;

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(fieldOfView, aspectRatio, 0.0, 2000.0);  /* Znear and Zfar */
    glViewport(0, 0, width, height);
}

void SceneViewer::paintGL()
{

    //qDebug() << Q_FUNC_INFO;
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    gluLookAt(0.f, 0.f, m_cameraZ, 0.f, 0.f, -5.f, 0.f, 1.f, 0.f);
    // set camera matrix
    //m_renderer->setCamera(m_cameraX, m_cameraY, m_cameraZ, 0, 0, 0);

    // rotate it around the y axis
    glRotatef(m_angle, 0.f,1.f,0.f);

    foreach(SceneNode *sn, nodes())
    {
        float tmp = sn->getScale();
        //QMessageBox msg;
        //msg.setText(QString("Model meshes: %1").arg(model->mNumMeshes));
        //msg.exec();

        // scale the whole asset to fit into our view frustum
        glScalef(tmp, tmp, tmp);

        // center the model
        aiVector3D sceneCenter = sn->getSceneCenter();
        glTranslatef( -sceneCenter.x, -sceneCenter.y, -sceneCenter.z);
        // if the display list has not been made yet, create a new one and
        // fill it with scene contents
        if(m_sceneList == 0)
        {
            m_sceneList = glGenLists(1);
            glNewList(m_sceneList, GL_COMPILE);
            // now begin at the root node of the imported data and traverse
            // the scenegraph by multiplying subsequent local transforms
            // together on GL's matrix stack.
            sn->draw();
            glEndList();
        }
        glCallList(m_sceneList);
    }

/*
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glColor3f (1.0, 1.0, 1.0);
    glBegin(GL_TRIANGLES);
    glVertex3f( 0.0f, 1.0f, 0.0f);
    glVertex3f(-1.0f,-1.0f, 0.0f);
    glVertex3f( 1.0f,-1.0f, 0.0f);
    glEnd();
*/
}

void SceneViewer::wheelEvent(QWheelEvent *event)
{
    m_cameraZ += event->delta() / 100;
    repaint();
}
