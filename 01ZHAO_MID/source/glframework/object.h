#pragma once
#include "core.h"

class Object
{
    public:
    Object();
    ~Object();
    //设置位置
    void setPoition(glm::vec3 pos);

    //设置旋转
    void rotateX(float angle);
    void rotateY(float angle);
    void rotateZ(float angle);

    //设置缩放
    void setScale(glm::vec3 scale);

    glm::mat4 getModelMatrix(); //获取模型矩阵

    protected:
    glm::vec3 mPosition{0.0f};
    //欧拉角
    float mAngleX{0.0f};
    float mAngleY{0.0f};
    float mAngleZ{0.0f};

    glm::vec3 mScale{1.0f};
};