#include "object.h"
#include <glm/ext/matrix_transform.hpp>
Object::Object()
{

}
Object::~Object()
{

}
//设置位置
void Object::setPosition(glm::vec3 pos)
{
    mPosition = pos;
}

//设置旋转
void Object::rotateX(float angle)
{
    mAngleX += angle;
}
void Object::rotateY(float angle)
{
    mAngleY += angle;
}
void Object::rotateZ(float angle)
{
    mAngleZ += angle;
}

//设置缩放
void Object::setScale(glm::vec3 scale)
{
    mScale = scale;
}

glm::mat4 Object::getModelMatrix() //获取模型矩阵
{
    //缩放旋转平移
    glm::mat4 transform{1.0f};
    transform = glm::scale(transform, mScale);

    //unity旋转标准
    transform  =glm::rotate(transform, glm::radians(mAngleX), glm::vec3(1.0f, 0.0f, 0.0f));
    transform  =glm::rotate(transform, glm::radians(mAngleY), glm::vec3(0.0f, 1.0f, 0.0f));
    transform  =glm::rotate(transform, glm::radians(mAngleZ), glm::vec3(0.0f, 0.0f, 1.0f));

    transform = glm::translate(glm::mat4(1.0f),mPosition)*transform;
    return transform;
}
