#include "texture.h"
#include "core.h"

#define STB_IMAGE_IMPLEMENTATION
#include "../application/stb_image.h"
Texture::Texture(const std::string&path,unsigned int unit)
{
    //加载图片
    mUnit = unit;
    //通道
    int channels;

    stbi_set_flip_vertically_on_load(true);

    unsigned char*data = stbi_load(path.c_str(),&mWidth,&mHeight,&channels,STBI_rgb_alpha);

    //生成纹理
    if(data)
    {
        glGenTextures(1,&mUnit);
        glActiveTexture(GL_TEXTURE0+mUnit);
        glBindTexture(GL_TEXTURE_2D,mUnit);
        
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

        glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,mWidth,mHeight,0,GL_RGBA,GL_UNSIGNED_BYTE,data);
        glGenerateMipmap(GL_TEXTURE_2D);

        stbi_image_free(data);
    }

}
Texture::~Texture()
{
    if(mTexture != 0)
    {
        glDeleteTextures(1,&mTexture);
    }
}

void Texture::bind()
{
    //激活纹理单元
    glActiveTexture(GL_TEXTURE0+mUnit);
    glBindTexture(GL_TEXTURE_2D,mTexture);
}
