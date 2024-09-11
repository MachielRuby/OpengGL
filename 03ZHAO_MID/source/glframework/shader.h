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
    void setFloat(const std::string& name, float value);

    void setVec2(const char* name, float x, float y) const;
    void setVector3(const std::string& name, float x, float y, float z);
	void setVector3(const std::string& name, const float* values);
	void setVector3(const std::string& name, const glm::vec3 value);

    void setVec4(const char* name, float x, float y, float z, float w) const;
    void setMat4(const char* name, const glm::mat4& mat) const;
    void setMat3(const char* name, const glm::mat3& mat) const;
    private:
    GLuint mProgram{0};

    void checkShaderErrors(GLuint shader,std::string type);
};

#endif