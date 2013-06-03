#include "view.h"
#include "math_3d.h"
#include "shaderdata.h"

#include <QDebug>

view* curView;

view::view()
{
    //Matrix4f translation, rotation, projection;

    viewProjectionMatrix.InitTranslationTransform(0.0f,1.0f,0.0f);

    qDebug() << Q_FUNC_INFO << "GENERATED MATRIX";
    qDebug() << viewProjectionMatrix.m[0][0] << viewProjectionMatrix.m[0][1] << viewProjectionMatrix.m[0][2] << viewProjectionMatrix.m[0][3];
    qDebug() << viewProjectionMatrix.m[1][0] << viewProjectionMatrix.m[1][1] << viewProjectionMatrix.m[1][2] << viewProjectionMatrix.m[1][3];
    qDebug() << viewProjectionMatrix.m[2][0] << viewProjectionMatrix.m[2][1] << viewProjectionMatrix.m[2][2] << viewProjectionMatrix.m[2][3];
    qDebug() << viewProjectionMatrix.m[3][0] << viewProjectionMatrix.m[3][1] << viewProjectionMatrix.m[3][2] << viewProjectionMatrix.m[3][3];
}

void view::bind()
{
    curView = this;
}

void view::insertViewProjectionMatrix()
{
    ShaderData::UniformMatrix4fv(MatViewProjection, curView->viewProjectionMatrix);
}
