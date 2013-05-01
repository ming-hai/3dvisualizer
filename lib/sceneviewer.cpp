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

    m_scaleFactor = 1.f;

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

const char *SceneViewer::fileToBuffer(QString filename)
{
    QFile file(filename);
    QString content;
    if (file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QTextStream in(&file);
        content = in.readAll();
        return content.toStdString().c_str();
    }

    return NULL;
}

void SceneViewer::initializeGL()
{
    qDebug() << Q_FUNC_INFO;
    int IsCompiled_VS, IsCompiled_FS;
    int IsLinked;
    int maxLength;
    char *vertexInfoLog;
    char *fragmentInfoLog;
    char *shaderProgramInfoLog;

    /* These pointers will receive the contents of our shader source code files */
    const GLchar *vertexsource, *fragmentsource;

    /* These are handles used to reference the shaders */
    GLuint vertexshader, fragmentshader;

    /* This is a handle to the shader program */
    GLuint shaderprogram;

    int argc = 0;
    glutInit(&argc, NULL);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);

    qDebug() << "Vendor: " << glGetString (GL_VENDOR);
    qDebug() << "Renderer: " << glGetString (GL_RENDERER);
    qDebug() << "Version: " << glGetString (GL_VERSION);
    qDebug() << "GLSL: " << glGetString (GL_SHADING_LANGUAGE_VERSION);

    //glewExperimental = GL_TRUE;
    GLenum res = glewInit();
    if (res != GLEW_OK)
    {
        qDebug() <<  "Error: " << glewGetErrorString(res);
        return;
    }
#if 0
    /* Allocate and assign a Vertex Array Object to our handle */
    glGenVertexArrays(1, &vao);

    /* Bind our Vertex Array Object as the current used object */
    glBindVertexArray(vao);

    /* Read our shaders into the appropriate buffers */
    vertexsource = fileToBuffer("output/tutorial2.vert");
    if (vertexsource == NULL)
        qDebug() << "Vertex shader not found !";
    fragmentsource = fileToBuffer("output/tutorial2.frag");
    if (fragmentsource == NULL)
        qDebug() << "Fragment shader not found !";

    /* Create an empty vertex shader handle */
    vertexshader = glCreateShader(GL_VERTEX_SHADER);

    /* Send the vertex shader source code to GL */
    /* Note that the source code is NULL character terminated. */
    /* GL will automatically detect that therefore the length info can be 0 in this case (the last parameter) */
    glShaderSource(vertexshader, 1, (const GLchar**)&vertexsource, 0);

    /* Compile the vertex shader */
    glCompileShader(vertexshader);

    glGetShaderiv(vertexshader, GL_COMPILE_STATUS, &IsCompiled_VS);
    if(IsCompiled_VS == FALSE)
    {
       glGetShaderiv(vertexshader, GL_INFO_LOG_LENGTH, &maxLength);

       /* The maxLength includes the NULL character */
       vertexInfoLog = (char *)malloc(maxLength);

       glGetShaderInfoLog(vertexshader, maxLength, &maxLength, vertexInfoLog);

       /* Handle the error in an appropriate way such as displaying a message or writing to a log file. */
       /* In this simple program, we'll just leave */
       free(vertexInfoLog);
       return;
    }

    /* Create an empty fragment shader handle */
    fragmentshader = glCreateShader(GL_FRAGMENT_SHADER);

    /* Send the fragment shader source code to GL */
    /* Note that the source code is NULL character terminated. */
    /* GL will automatically detect that therefore the length info can be 0 in this case (the last parameter) */
    glShaderSource(fragmentshader, 1, (const GLchar**)&fragmentsource, 0);

    /* Compile the fragment shader */
    glCompileShader(fragmentshader);

    glGetShaderiv(fragmentshader, GL_COMPILE_STATUS, &IsCompiled_FS);
    if(IsCompiled_FS == FALSE)
    {
       glGetShaderiv(fragmentshader, GL_INFO_LOG_LENGTH, &maxLength);

       /* The maxLength includes the NULL character */
       fragmentInfoLog = (char *)malloc(maxLength);

       glGetShaderInfoLog(fragmentshader, maxLength, &maxLength, fragmentInfoLog);

       /* Handle the error in an appropriate way such as displaying a message or writing to a log file. */
       /* In this simple program, we'll just leave */
       free(fragmentInfoLog);
       return;
    }

    /* If we reached this point it means the vertex and fragment shaders compiled and are syntax error free. */
    /* We must link them together to make a GL shader program */
    /* GL shader programs are monolithic. It is a single piece made of 1 vertex shader and 1 fragment shader. */
    /* Assign our program handle a "name" */
    shaderprogram = glCreateProgram();

    /* Attach our shaders to our program */
    glAttachShader(shaderprogram, vertexshader);
    glAttachShader(shaderprogram, fragmentshader);

    /* Bind attribute index 0 (coordinates) to in_Position and attribute index 1 (color) to in_Color */
    /* Attribute locations must be setup before calling glLinkProgram. */
    glBindAttribLocation(shaderprogram, 0, "in_Position");
    glBindAttribLocation(shaderprogram, 1, "in_Color");

    /* Link our program */
    /* At this stage, the vertex and fragment programs are inspected, optimized and a binary code is generated for the shader. */
    /* The binary code is uploaded to the GPU, if there is no error. */
    glLinkProgram(shaderprogram);

    /* Again, we must check and make sure that it linked. If it fails, it would mean either there is a mismatch between the vertex */
    /* and fragment shaders. It might be that you have surpassed your GPU's abilities. Perhaps too many ALU operations or */
    /* too many texel fetch instructions or too many interpolators or dynamic loops. */

    glGetProgramiv(shaderprogram, GL_LINK_STATUS, (int *)&IsLinked);
    if(IsLinked == FALSE)
    {
       /* Noticed that glGetProgramiv is used to get the length for a shader program, not glGetShaderiv. */
       glGetProgramiv(shaderprogram, GL_INFO_LOG_LENGTH, &maxLength);

       /* The maxLength includes the NULL character */
       shaderProgramInfoLog = (char *)malloc(maxLength);

       /* Notice that glGetProgramInfoLog, not glGetShaderInfoLog. */
       glGetProgramInfoLog(shaderprogram, maxLength, &maxLength, shaderProgramInfoLog);

       /* Handle the error in an appropriate way such as displaying a message or writing to a log file. */
       /* In this simple program, we'll just leave */
       free(shaderProgramInfoLog);
       return;
    }

    /* Load the shader into the rendering pipeline */
    glUseProgram(shaderprogram);
#endif

/*
    Vector3f Pos(3.0f, 7.0f, -10.0f);
    Vector3f Target(0.0f, -0.2f, 1.0f);
    Vector3f Up(0.0, 1.0f, 0.0f);

    m_camera = new Camera(width(), height(), Pos, Target, Up);
*/
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
    //m_camera->OnRender();

    //glClearColor(0.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    foreach(SceneNode *sn, nodes())
    {
        sn->render();
    }
    //this->swapBuffers();
    //glutSwapBuffers();

/*
    // Make our background black
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);

    // Invoke glDrawArrays telling that our data is a line loop and we want to draw 2-4 vertexes
    glDrawArrays(GL_LINE_LOOP, 0, 4);
*/

/*
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
*/


/*
    // CUBE for dummies
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
