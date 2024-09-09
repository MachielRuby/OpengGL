#include "shader.h"
#include "../wrapper/checkError.h"

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
    wcas
   
}
Shader::~Shader()
{

}

void Shader::begin()//使用
{
    
}
void Shader::end()//结束使用
{

}