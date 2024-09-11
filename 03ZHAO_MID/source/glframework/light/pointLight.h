#pragma once
#include "light.h"
#include "../object.h"

class PointLight:public Object, public Light
{
    public:
    PointLight();
    ~PointLight();

    public:
    float mK2 = 1.0f;
    float mK1 = 0.0f;
    float mK0 = 0.0f;
};