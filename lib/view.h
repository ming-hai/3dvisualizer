#ifndef VIEW_H
#define VIEW_H

#include "math_3d.h"

//extern static view* curView;

class view
{
public:
    view();
    void bind();
    static void insertViewProjectionMatrix();
private:
    Matrix4f viewProjectionMatrix;
};

#endif // VIEW_H
