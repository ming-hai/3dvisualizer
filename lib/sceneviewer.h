/*
  Q Light Controller Plus
  sceneviewer.h

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

#ifndef SCENEVIEWER_H
#define SCENEVIEWER_H

#include "GL/glew.h"
#include "GL/glut.h"

#include <QGLWidget>

#include "deferredrendering.h"
#include "fborendertexture.h"
#include "staticspotlight.h"
#include "scenenode.h"
#include "viewport.h"

class SceneViewer : public QGLWidget
{
    Q_OBJECT
public:
    SceneViewer(QWidget *parent = 0);
    ~SceneViewer();

    /*********************************************************************
     * Node Instances
     *********************************************************************/
protected:
    quint32 createNodeId();

public:
    /**
     * Add the given node to SceneViewer's nodes array.
     * If id == SceneNode::invalidId(), SceneViewer assigns the node a new ID and
     * takes ownership, unless there is no more room for more nodes.
     *
     * If id != SceneViewer::invalidId(), SceneViewer attempts to put the node at
     * that exact index, unless another node already occupies it.
     *
     * @param path The source path of the node to add
     * @param id The requested ID for the node
     * @return true if the node was successfully added to SceneViewer,
     *         otherwise false.
     */
    quint32 addNode(QString path, quint32 id = SceneNode::invalidId());

    quint32 addSpotlight(quint32 id = SceneNode::invalidId());

    /**
     * Delete the given node instance from SceneViewer
     *
     * @param id The ID of the node instance to delete
     */
    bool deleteNode(quint32 id);

    /**
     * Get the SceneNode instance that has the given ID
     *
     * @param id The ID of the SceneNode to get
     */
    SceneNode* node(quint32 id) const;

    /**
     * Get the list of currently loaded nodes
     */
    QList <SceneNode*> nodes() const;

protected:
    /** Nodes map paired by unique IDs */
    QMap <quint32,SceneNode*> m_nodes;

    /** Latest assigned node ID */
    quint32 m_latestNodeId;

    /*********************************************************************
     * Lights
     *********************************************************************/
    /** Lights map paired by unique IDs */
    QMap <quint32, Spotlight*>m_sceneLights;

    /*********************************************************************
     * OpenGL
     *********************************************************************/
protected:

    /** @reimp */
    void initializeGL();

    /** @reimp */
    void resizeGL(int width, int height);

    /** @reimp */
    void paintGL();

    /** @reimp */
    void wheelEvent(QWheelEvent *event);

private:
    float m_scale;
    bool m_debugBuffers;

    int m_width, m_height;

    Shader *m_modelsShader; // common shader for all the models
    ViewPort *m_viewPort;
    DeferredRendering*	m_deferredRendering;

signals:
    void nodeChanged(quint32 id);
    
public slots:

    
};

#endif // SCENEVIEWER_H
