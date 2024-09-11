#pragma once

#include "object.h"
#include "geometry.h"
#include "material/material.h"

class Mesh:public Object
{
public:
    Mesh(Geometry*geometry,Material*material);;
    ~Mesh();

    public: //物体材质
    Geometry* mGeometry{nullptr};
    Material* mMaterial{nullptr};
};