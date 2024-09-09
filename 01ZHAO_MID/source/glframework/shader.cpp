#include "shader.h"
#include "../wrapper/checkError.h"
#include "core.h"

#include<string>
#include<fstream>
#include<sstream>
#include<iostream>

Shader::Shader(const char* vertexPath, const char* fragmentPath)
{
    //读取文件到
    std::string vertexCode; 
    std::string fragmentCode;
    //
    std::ifstream vShaderFile;
    std::ifstream fShaderFile;

    vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    try
    {
        vShaderFile.open(vertexPath);
        fShaderFile.open(fragmentPath);

        //读取文件内容
        std::stringstream vShaderStream, fShaderStream;
        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();

        //关闭文件
        vShaderFile.close();
        fShaderFile.close();

        //转化为字符串
        vertexCode = vShaderStream.str();
        fragmentCode = fShaderStream.str();
    }
    catch(std::ifstream::failure&e)
    {
        std::cout<<"Error::SHADER::FILE_NOT_SUCCESFULLY_READ: "<<e.what()<<std::endl;
    }

    //编译着色器
    const char*vertexShaderSource = vertexCode.c_str();
    const char*fragmentShaderSource = fragmentCode.c_str();
    //顶点着色器
    GLuint vertex,fragment;
    vertex  = glCreateShader(GL_VERTEX_SHADER);
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    
    //编译顶点着色器
    glShaderSource(vertex,1,&vertexShaderSource,NULL); //参数：着色器对象，着色器类型，着色器源码，源码长度
    glShaderSource(fragment,1,&fragmentShaderSource,NULL);
     
    //编译着色器
    glCompileShader(vertex);
    checkShaderErrors(vertex,"COMPILE");

    glCompileShader(fragment);
    checkShaderErrors(fragment,"COMPILE");

    //链接着色器
    mProgram = glCreateProgram();

    //链接顶点着色器
    glAttachShader(mProgram,vertex);
    glAttachShader(mProgram,fragment);

    glLinkProgram(mProgram);

    checkShaderErrors(mProgram, "LINK");

    glDeleteShader(vertex);
    glDeleteShader(fragment);
}
Shader::~Shader()
{

}

void Shader::begin()//使用
{
    GL_CALL(glUseProgram(mProgram));
}
void Shader::end()//结束使用
{
    GL_CALL(glUseProgram(0));
}

void Shader::checkShaderErrors(GLuint target,std::string type)
{
    int success = 0;
    char infoLog[1024];

    if(type == "COMPILE")
    {
        glGetShaderiv(target,GL_COMPILE_STATUS,&success);
        if(!success)
        {
            //获取错误信息
            glGetShaderInfoLog(target,1024,NULL,infoLog);
            std::cout<<"ERROR::SHADER::"<<type<<": "<<infoLog<<std::endl;
        }
        else if(type == "LINK")
        {
            glGetProgramiv(target,GL_LINK_STATUS,&success);
            if(!success)
            {
                //获取错误信息
                glGetProgramInfoLog(target,1024,NULL,infoLog);
                std::cout<<"ERROR::SHADER::"<<type<<": "<<infoLog<<std::endl;
            }
        }
        else {
            std::cout<<"ERROR::Check Shader Errors: unknown";
        }
    }
}


void Shader::setBool(const char* name, bool value) const
{
    GLuint location = glGetUniformLocation(mProgram,name);
    glUniform1i(location,value);
}
void Shader::setInt(const char* name, int value) const
{
    GLuint location = glGetUniformLocation(mProgram,name);
    glUniform1i(location,value);
}
void Shader::setFloat(const char* name, float value) const
{
    GLuint location = glGetUniformLocation(mProgram,name);
    glUniform1f(location,value);
}
void Shader::setVec2(const char* name, float x, float y) const
{
    GLuint location = glGetUniformLocation(mProgram,name);
    glUniform2f(location,x,y);
}
void Shader::setVec3(const char* name, float x, float y, float z) const
{
    GLuint location = glGetUniformLocation(mProgram,name);
    glUniform3f(location,x,y,z);
}
void Shader::setVec4(const char* name, float x, float y, float z, float w) const
{
    GLuint location = glGetUniformLocation(mProgram,name);
    glUniform4f(location,x,y,z,w);
}
