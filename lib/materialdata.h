#ifndef MATERIALDATA_H
#define MATERIALDATA_H

#include "shaderdata.h"

class materialdata
{
public:
    materialdata();
    bool bind(enum DrawingPass pass);
private:
    ShaderData* normalShader;
};

#endif // MATERIALDATA_H
