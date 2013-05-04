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

#include "pipeline.h"

SceneViewer::SceneViewer(QWidget *parent)
    : QGLWidget(parent)
{
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    //m_renderer = new GLRenderer();

    m_scaleFactor = 0.0f;

    m_camera = NULL;
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

    //glEnable(GL_DEPTH_TEST);
    m_persProjInfo.FOV = 60.0f;
    m_persProjInfo.Height = height();
    m_persProjInfo.Width = width();
    m_persProjInfo.zNear = 1.0f;
    m_persProjInfo.zFar = 100.0f;

    m_spotLight.AmbientIntensity = 0.0f;
    m_spotLight.DiffuseIntensity = 0.9f;
    m_spotLight.Color = COLOR_WHITE;
    m_spotLight.Attenuation.Linear = 0.01f;
    m_spotLight.Position  = Vector3f(-20.0, 20.0, 5.0f);
    m_spotLight.Direction = Vector3f(1.0f, -1.0f, 0.0f);
    m_spotLight.Cutoff =  20.0f;

    m_dirLight.AmbientIntensity = 0.1f;
    m_dirLight.Color = COLOR_CYAN;
    m_dirLight.DiffuseIntensity = 0.5f;
    m_dirLight.Direction = Vector3f(1.0f, 0.0f, 0.0f);

    if (!m_gbuffer.Init(width(), height()))
        return;

    m_camera = new Camera(width(), height());

    if (!m_DSGeomPassTech.Init())
    {
        qDebug() << "Error initializing DSGeomPassTech";
        return;
    }

    m_DSGeomPassTech.Enable();
    m_DSGeomPassTech.SetColorTextureUnit(COLOR_TEXTURE_UNIT_INDEX);

    if (!m_DSPointLightPassTech.Init())
    {
        qDebug() << "Error initializing DSPointLightPassTech";
        return;
    }

    m_DSPointLightPassTech.Enable();

    m_DSPointLightPassTech.SetPositionTextureUnit(GBuffer::GBUFFER_TEXTURE_TYPE_POSITION);
    m_DSPointLightPassTech.SetColorTextureUnit(GBuffer::GBUFFER_TEXTURE_TYPE_DIFFUSE);
    m_DSPointLightPassTech.SetNormalTextureUnit(GBuffer::GBUFFER_TEXTURE_TYPE_NORMAL);
    m_DSPointLightPassTech.SetScreenSize(width(), height());

    if (!m_DSDirLightPassTech.Init())
    {
        qDebug() << "Error initializing DSDirLightPassTech";
        return;
    }

    m_DSDirLightPassTech.Enable();

    m_DSDirLightPassTech.SetPositionTextureUnit(GBuffer::GBUFFER_TEXTURE_TYPE_POSITION);
    m_DSDirLightPassTech.SetColorTextureUnit(GBuffer::GBUFFER_TEXTURE_TYPE_DIFFUSE);
    m_DSDirLightPassTech.SetNormalTextureUnit(GBuffer::GBUFFER_TEXTURE_TYPE_NORMAL);
    m_DSDirLightPassTech.SetDirectionalLight(m_dirLight);
    m_DSDirLightPassTech.SetScreenSize(width(), height());
    Matrix4f WVP;
    WVP.InitIdentity();
    m_DSDirLightPassTech.SetWVP(WVP);

    if (!m_nullTech.Init()) {
        return;
    }
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
}

void SceneViewer::paintGL()
{
    m_camera->OnRender();

    //glClearColor(0.0, 0.0, 0.0, 1.0);
    //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    m_gbuffer.StartFrame();

    DSGeometryPass();

    // We need stencil to be enabled in the stencil pass to get the stencil buffer
    // updated and we also need it in the light pass because we render the light
    // only if the stencil passes.
/*
    glEnable(GL_STENCIL_TEST);

    for (unsigned int i = 0 ; i < ARRAY_SIZE_IN_ELEMENTS(m_pointLight); i++)
    {
        DSStencilPass(i);
        DSPointLightPass(i);
    }

    // The directional light does not need a stencil test because its volume
    // is unlimited and the final pass simply copies the texture.
    glDisable(GL_STENCIL_TEST);
*/
    DSDirectionalLightPass();

    DSFinalPass();

    //this->swapBuffers();
    //glutSwapBuffers();
}

void SceneViewer::DSGeometryPass()
{
    m_DSGeomPassTech.Enable();

    m_gbuffer.BindForGeomPass();

    // Only the geometry pass updates the depth buffer
    glDepthMask(GL_TRUE);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glEnable(GL_DEPTH_TEST);

    Pipeline p;
    p.SetCamera(m_camera->GetPos(), m_camera->GetTarget(), m_camera->GetUp());
    p.SetPerspectiveProj(m_persProjInfo);
    p.Rotate(0.0f, m_scaleFactor, 0.0f);

    foreach(SceneNode *sn, nodes())
    {
        p.WorldPos(Vector3f(0.0f, 0.0f, 5.0f));
        m_DSGeomPassTech.SetWVP(p.GetWVPTrans());
        m_DSGeomPassTech.SetWorldMatrix(p.GetWorldTrans());
        sn->render();
    }
/*
    for (unsigned int i = 0 ; i < ARRAY_SIZE_IN_ELEMENTS(m_boxPositions) ; i++)
    {
        p.WorldPos(m_boxPositions[i]);
        m_DSGeomPassTech.SetWVP(p.GetWVPTrans());
        m_DSGeomPassTech.SetWorldMatrix(p.GetWorldTrans());
        m_box.Render();
    }
*/
    // When we get here the depth buffer is already populated and the stencil pass
    // depends on it, but it does not write to it.
    glDepthMask(GL_FALSE);
}

/*
void SceneViewer::DSStencilPass(unsigned int PointLightIndex)
{
    m_nullTech.Enable();

    // Disable color/depth write and enable stencil
    m_gbuffer.BindForStencilPass();
    glEnable(GL_DEPTH_TEST);

    glDisable(GL_CULL_FACE);

    glClear(GL_STENCIL_BUFFER_BIT);

    // We need the stencil test to be enabled but we want it
    // to succeed always. Only the depth test matters.
    glStencilFunc(GL_ALWAYS, 0, 0);

    glStencilOpSeparate(GL_BACK, GL_KEEP, GL_INCR, GL_KEEP);
    glStencilOpSeparate(GL_FRONT, GL_KEEP, GL_DECR, GL_KEEP);

    Pipeline p;
    p.WorldPos(m_pointLight[PointLightIndex].Position);
    float BBoxScale = CalcPointLightBSphere(m_pointLight[PointLightIndex].Color,
                                         m_pointLight[PointLightIndex].DiffuseIntensity);
    p.Scale(BBoxScale, BBoxScale, BBoxScale);
    p.SetCamera(m_pGameCamera->GetPos(), m_pGameCamera->GetTarget(), m_pGameCamera->GetUp());
    p.SetPerspectiveProj(m_persProjInfo);

    m_nullTech.SetWVP(p.GetWVPTrans());
    //m_bsphere.Render();
}


void SceneViewer::DSPointLightPass(unsigned int PointLightIndex)
{
    m_gbuffer.BindForLightPass();

    m_DSPointLightPassTech.Enable();
    m_DSPointLightPassTech.SetEyeWorldPos(m_pGameCamera->GetPos());

    glStencilFunc(GL_NOTEQUAL, 0, 0xFF);

    glDisable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendEquation(GL_FUNC_ADD);
    glBlendFunc(GL_ONE, GL_ONE);

    glEnable(GL_CULL_FACE);
    glCullFace(GL_FRONT);

    Pipeline p;
    p.WorldPos(m_pointLight[PointLightIndex].Position);
    float BBoxScale = CalcPointLightBSphere(m_pointLight[PointLightIndex].Color,
                                            m_pointLight[PointLightIndex].DiffuseIntensity);
    p.Scale(BBoxScale, BBoxScale, BBoxScale);
    p.SetCamera(m_pGameCamera->GetPos(), m_pGameCamera->GetTarget(), m_pGameCamera->GetUp());
    p.SetPerspectiveProj(m_persProjInfo);
    m_DSPointLightPassTech.SetWVP(p.GetWVPTrans());
    m_DSPointLightPassTech.SetPointLight(m_pointLight[PointLightIndex]);
    //m_bsphere.Render();
    glCullFace(GL_BACK);

    glDisable(GL_BLEND);
}
*/

void SceneViewer::DSDirectionalLightPass()
{
    m_gbuffer.BindForLightPass();

    m_DSDirLightPassTech.Enable();
    m_DSDirLightPassTech.SetEyeWorldPos(m_camera->GetPos());

    glDisable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendEquation(GL_FUNC_ADD);
    glBlendFunc(GL_ONE, GL_ONE);

    foreach(SceneNode *sn, nodes())
        sn->render();

    glDisable(GL_BLEND);
}


void SceneViewer::DSFinalPass()
{
    m_gbuffer.BindForFinalPass();
    glBlitFramebuffer(0, 0, width(), height(),
                      0, 0, width(), height(), GL_COLOR_BUFFER_BIT, GL_LINEAR);
}

void SceneViewer::wheelEvent(QWheelEvent *event)
{
    m_cameraZ += event->delta() / 100;
    repaint();
}
