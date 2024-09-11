#pragma once
#include "../core.h"

enum class MaterialType
{
    PhongMaterial
};

class Material
{
    public:
    Material();
    ~Material();
    public:
    MaterialType mType;
};