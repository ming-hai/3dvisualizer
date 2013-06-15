/*
  Q Light Controller Plus
  filter2d.h

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

#ifndef FILTER2D_H
#define FILTER2D_H

#include "scenenode.h"

class Filter2D: public SceneNode
{
public:
	Filter2D(void);
	~Filter2D(void);
	void Draw(MaterialData* filterMaterial);
    void Draw(MaterialData* filterMaterial, QList<UniformInsert *> UniformInsterts);
};

#endif

