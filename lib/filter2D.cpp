/*
  Q Light Controller Plus
  filter2d.cpp

  Copyright (C) Phillip Schulz-Theißen, Massimo Callegari

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

#include "filter2D.h"

Filter2D::Filter2D(void)
{
    //Mesh = MeshData::FromObj("screenQuad.obj");

    QString screenQuad =
            "mtllib floor.mtl\n"
            "o Plane\n"
            "v 1.000000 -1.000000 0.000000\n"
            "v -1.000000 -1.000000 0.000000\n"
            "v 1.000000 1.000000 0.000000\n"
            "v -1.000000 1.000000 0.000000\n"
            "vt 0.000000 1.000000\n"
            "vt 1.000000 1.000000\n"
            "vt 1.000000 0.000000\n"
            "vt 0.000000 0.000000\n"
            "vn 0.000000 0.000000 1.000000\n"
            "usemtl\n"
            "s off\n"
            "f 2/1/1 1/2/1 3/3/1\n"
            "f 2/1/1 3/3/1 4/4/1\n";
    loadModelFromBuffer(screenQuad);
}


Filter2D::~Filter2D(void)
{
}

void Filter2D::Draw(MaterialData* filterMaterial)
{
    bind();

    if(!filterMaterial->bind(DrawingPassSolid))
		return;

    glDrawElements(GL_TRIANGLES, getSize(), GL_UNSIGNED_INT, 0);
}

void Filter2D::Draw(MaterialData* filterMaterial, QList<UniformInsert *> UniformInsterts)
{
    bind();

    if(!filterMaterial->bind(DrawingPassSolid))
		return;

	ShaderData::ParseUniformInserts(UniformInsterts);

    glDrawElements(GL_TRIANGLES, getSize(), GL_UNSIGNED_INT, 0);
}

