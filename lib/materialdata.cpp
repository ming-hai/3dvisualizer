#include "materialdata.h"
#include "shaderdata.h"

materialdata::materialdata()
{
    normalShader = ShaderData::FromPlainText(
                "C:\\Users\\ultradr3mer\\visualizer\\shaders\\normal.vs",
                "C:\\Users\\ultradr3mer\\visualizer\\shaders\\normal.fs");
}

bool materialdata::bind(enum DrawingPass pass)
{
    normalShader->Bind();
    return true;
}
