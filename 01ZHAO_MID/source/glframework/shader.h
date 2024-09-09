#ifndef SHADER_H
#define SHADER_H

#include "core.h"
#include<string>

class Shader{
    public:
    Shader(const char* vertexPath, const char* fragmentPath);
    ~Shader();

    void begin();//使用
    void end();//结束使用
    
    void setBool(const char* name, bool value) const;
    void setInt(const char* name, int value) const;
    void setFloat(const char* name, float value) const;
    void setVec2(const char* name, float x, float y) const;
    void setVec3(const char* name, float x, float y, float z) const;
    void setVec4(const char* name, float x, float y, float z, float w) const;

    private:
    GLuint mProgram{0};
};

#endif