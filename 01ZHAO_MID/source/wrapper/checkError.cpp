#include "checkError.h"

#include <glad/glad.h>
#include<iostream>
#include<assert.h>
void checkError()
{
    GLenum errorCode = glGetError();
    std::string error = "";
    if(errorCode != GL_NO_ERROR)
    {
        switch (errorCode) {
            
            case GL_INVALID_ENUM: error = "GL_INVALID_ENUM";break;
            case GL_INVALID_VALUE: error = "GL_INVALID_VALUE";break;
            case GL_INVALID_OPERATION: error = "GL_INVALID_OPERATION";break;
            case GL_INVALID_FRAMEBUFFER_OPERATION: error = "GL_INVALID_FRAMEBUFFER_OPERATION";break;
            case GL_OUT_OF_MEMORY: error = "GL_OUT_OF_MEMORY";break;
            default:
                error = "Unknown error";
                break;

        }
        std::cout<<"error"<<std::endl;
        //assert 终止程序
        assert(false);
    }
}