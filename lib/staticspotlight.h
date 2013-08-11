/*
  Q Light Controller Plus
  staticspotlight.h

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

#ifndef STATICSPOTLIGHT_H
#define STATICSPOTLIGHT_H

#include "scenenode.h"
#include "spotlight.h"

class StaticSpotlight : public SceneNode
{
public:
    StaticSpotlight(ViewPort *view, SceneNode *parent = 0);

    void attachLight(Spotlight *light);

private:
    Spotlight *m_light;
};

#endif // STATICSPOTLIGHT_H
