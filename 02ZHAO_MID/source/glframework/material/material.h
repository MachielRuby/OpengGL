#pragma once
#include "../core.h"

enum class MaterialType
{
    PhongMaterial,
    WhiteMaterial
};

class Material
{
    public:
    Material();
    ~Material();
    public:
    MaterialType mType;
};