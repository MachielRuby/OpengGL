#include "renderer.h"
#include <glm/trigonometric.hpp>
#include <iostream>
#include "../material/phongMaterial.h"
#include "../material/whiteMaterial.h"
#include<string>
Renderer::Renderer()
{
	mPhongShader = new Shader(R"(E:\CPP_ALL\02OPENGL\03ZHAO_MID\build\assets\assets\shaders\06\phong.vert)", R"(E:\CPP_ALL\02OPENGL\03ZHAO_MID\build\assets\assets\shaders\06\phong.frag)");
    mWhiteShader = new Shader(R"(E:\CPP_ALL\02OPENGL\03ZHAO_MID\build\assets\assets\shaders\03\white.vert)", R"(E:\CPP_ALL\02OPENGL\03ZHAO_MID\build\assets\assets\shaders\03\white.frag)");
}
Renderer::~Renderer()
{

}
void Renderer::render(
	Scene* scene, 
	Camera* camera,
	DirectionalLight* dirLight,
	AmbientLight* ambLight
) {
	//1 设置当前帧绘制的时候，opengl的必要状态机参数
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	//2 清理画布 
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//3 将scene当作根节点开始递归渲染
	renderObject(scene, camera, dirLight, ambLight);
}

Shader* Renderer::pickShader(MaterialType type) {
	Shader* result = nullptr;

	switch (type) {
	case MaterialType::PhongMaterial:
		result = mPhongShader;
		break;
	case MaterialType::WhiteMaterial:
		result = mWhiteShader;
		break;
	default:
		std::cout << "Unknown material type to pick shader" << std::endl;
		break;
	}

	return result;
}


//针对单个object进行渲染
void Renderer::renderObject(
	Object* object,
	Camera* camera,
	DirectionalLight* dirLight,
	AmbientLight* ambLight
) {
	//1 判断是Mesh还是Object，如果是Mesh需要渲染
	if (object->getType() == ObjectType::Mesh) {
		auto mesh = (Mesh*)object;
		auto geometry = mesh->mGeometry;
		auto material = mesh->mMaterial;

		//1 决定使用哪个Shader 
		Shader* shader = pickShader(material->mType);

		//2 更新shader的uniform
		shader->begin();

		switch (material->mType) {
		case MaterialType::PhongMaterial: {
			PhongMaterial* phongMat = (PhongMaterial*)material;
            
			//diffuse贴图帧更新
			//将纹理采样器与纹理单元进行挂钩
			shader->setInt("sampler", 0);
			//将纹理与纹理单元进行挂钩
			phongMat->mDiffuse->bind();

			//高光蒙版的帧更新
			shader->setInt("specularMaskSampler", 1);
			phongMat->mSpecularMask->bind();

			//mvp
			shader->setMat4("modelMatrix", mesh->getModelMatrix());
			shader->setMat4("viewMatrix", camera->getViewMatrix());
			shader->setMat4("projectionMatrix", camera->getProjectionMatrix());

			auto normalMatrix = glm::mat3(glm::transpose(glm::inverse(mesh->getModelMatrix())));
			shader->setMat3("normalMatrix", normalMatrix);

			//光源参数的uniform更新
			//directionalLight 的更新
			shader->setVector3("directionalLight.color", dirLight->mColor);
			shader->setVector3("directionalLight.direction", dirLight->mDirection);
			shader->setFloat("directionalLight.specularIntensity", dirLight->mSpecularIntensity);


			shader->setFloat("shiness", phongMat->mShiness);

			shader->setVector3("ambientColor", ambLight->mColor);

			//相机信息更新
			shader->setVector3("cameraPosition", camera->mPosition);

		}
										break;
		case MaterialType::WhiteMaterial: {
			//mvp
			shader->setMat4("modelMatrix", mesh->getModelMatrix());
			shader->setMat4("viewMatrix", camera->getViewMatrix());
			shader->setMat4("projectionMatrix", camera->getProjectionMatrix());
		}
										break;
		default:
			break;
		}

		//3 绑定vao
		glBindVertexArray(geometry->getVao());

		//4 执行绘制命令
		glDrawElements(GL_TRIANGLES, geometry->getIndicesCount(), GL_UNSIGNED_INT, 0);
	}


	//2 遍历object的子节点，对每个子节点都需要调用renderObject
	auto children = object->getChildren();
	for (int i = 0; i < children.size(); i++) {
		renderObject(children[i], camera, dirLight, ambLight);
	}
}

void Renderer::setClearColor(glm::vec3 color) {
	glClearColor(color.r, color.g, color.b, 1.0);
}