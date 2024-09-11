#include "renderer.h"
#include <glm/trigonometric.hpp>
#include <iostream>
#include "../material/phongMaterial.h"
#include "../material/whiteMaterial.h"
#include<string>
Renderer::Renderer()
{
	mPhongShader = new Shader(R"(E:\CPP_ALL\02OPENGL\02ZHAO_MID\build\assets\assets\shaders\06\phong.vert)", R"(E:\CPP_ALL\02OPENGL\02ZHAO_MID\build\assets\assets\shaders\06\phong.frag)");
    mWhiteShader = new Shader(R"(E:\CPP_ALL\02OPENGL\02ZHAO_MID\build\assets\assets\shaders\03\white.vert)", R"(E:\CPP_ALL\02OPENGL\02ZHAO_MID\build\assets\assets\shaders\03\white.frag)");
}
Renderer::~Renderer()
{

}
void Renderer::render(
    const std::vector<Mesh*>& meshes,
    Camera*camera,
    const std::vector<PointLight*>&pointLights,
    DirectionalLight*dirLight,
    SpotLight*spotLight,
    AmbientLight*ambLight
)
{
    if(pointLights.size() > 0){std::cout << "pointLights.size() = " << pointLights.size() << std::endl;}
    //设置当当前绘制的时候的必要状态机参数
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    //清理画布
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //mesh进行绘制
    for(int i = 0;i<meshes.size();i++)
    {
        auto mesh = meshes[i];
        auto geometry = mesh->mGeometry;
        auto material = mesh->mMaterial;

        //决心使用哪个shader
        Shader*shader = pickShader(material->mType);

        //设置shader的参数
        shader->begin();

        switch(material->mType)
        {
            case MaterialType::PhongMaterial:
            {
            PhongMaterial* phongMat = (PhongMaterial*)material;
			//diffuse贴图
			//将纹理采样器与纹理单元进行挂钩
			shader->setInt("sampler", 0);
			//将纹理与纹理单元进行挂钩
            
			phongMat->mDiffuse->bind();
            if(phongMat->mSpecularMask!=nullptr)
            {
            shader->setInt("specularMaskSampler", 1);
			phongMat->mSpecularMask->bind();

            }


			//mvp
			shader->setMat4("modelMatrix", mesh->getModelMatrix());
			shader->setMat4("viewMatrix", camera->getViewMatrix());
			shader->setMat4("projectionMatrix", camera->getProjectionMatrix());

			auto normalMatrix = glm::mat3(glm::transpose(glm::inverse(mesh->getModelMatrix())));
			shader->setMat3("normalMatrix", normalMatrix);

			//光源参数的uniform更新
           	shader->setVector3("spotLight.position", spotLight->getPosition());
			shader->setVector3("spotLight.color", spotLight->mColor);
			shader->setVector3("spotLight.targetDirection", spotLight->mTargetDircection);
			shader->setFloat("spotLight.specularIntensity", spotLight->mSpecularIntensity);
			shader->setFloat("spotLight.innerLine", glm::cos(glm::radians(spotLight->mInnerAngle)));
			shader->setFloat("spotLight.outerLine", glm::cos(glm::radians(spotLight->mOuterAngle)));
 
            //directionalLight 的更新
			shader->setVector3("directionalLight.color", dirLight->mColor);
			shader->setVector3("directionalLight.direction", dirLight->mDirection);
			shader->setFloat("directionalLight.specularIntensity", dirLight->mSpecularIntensity);

			//pointLight的更新
            for (int i = 0; i < pointLights.size(); i++) {
            auto pointLight = pointLights[i];

            std::string baseName = "pointLights[";
            baseName.append(std::to_string(i)); // e.g., pointLights[0]
            baseName.append("]");

            shader->setVector3(baseName + ".color", pointLight->mColor);
            shader->setVector3(baseName + ".position", pointLight->getPosition());
            shader->setFloat(baseName + ".specularIntensity", pointLight->mSpecularIntensity);
            shader->setFloat(baseName + ".k2", pointLight->mK2);
            shader->setFloat(baseName + ".k1", pointLight->mK1);
            shader->setFloat(baseName + ".k0", pointLight->mK0);
            }
\
		
			shader->setFloat("shiness", phongMat->mShiness);

			shader->setVector3("ambientColor", ambLight->mColor);

			//相机信息更新
			shader->setVector3("cameraPosition", camera->mPosition);
                break;
            }
            case MaterialType::WhiteMaterial:
            {
                shader->setMat4("modelMatrix", mesh->getModelMatrix());
                shader->setMat4("viewMatrix", camera->getViewMatrix());
                shader->setMat4("projectionMatrix", camera->getProjectionMatrix());
                break;
            }
            default:
                continue;
        }
        //绑定vao
        glBindVertexArray(geometry->getVao());
        glDrawElements(GL_TRIANGLES, geometry->getIndicesCount(),GL_UNSIGNED_INT, 0);
    }

}
//根据材质类型选择shader
Shader*Renderer::pickShader(MaterialType type)
{
    Shader*result  = nullptr;
    switch(type)
    {
        case MaterialType::PhongMaterial:
        result = mPhongShader;
        break;
        case MaterialType::WhiteMaterial:
        result = mWhiteShader;
        break;
        default:
            std::cout<<"unknown material type"<<std::endl;
            break;
    }
    return result;
}

void Renderer::setClearColor(glm::vec3 color) {
	glClearColor(color.r, color.g, color.b, 1.0);
}