#pragma once
#include<vector>
#include "../core.h"
#include "../mesh.h"
#include "../../application/camera/camera.h"
#include "../light/directionalLight.h"
#include "../light/ambientLight.h"
#include "../light/pointLight.h"
#include "../light/spotLight.h"
#include "../shader.h"
#include "../scene.h"

class Renderer
{
    public:
    Renderer();
    ~Renderer();

    //每次调用都会熏染
    void render(
        const std::vector<Mesh*>& meshes,
        Camera*camera,
        const std::vector<PointLight*>&pointLights,
        DirectionalLight*dirLight,
        SpotLight*spotLight,
        AmbientLight*ambLight
    );

    void setClearColor(glm::vec3 color = glm::vec3(0.2f, 0.4f, 0.5f));
    private:
    Shader*pickShader(MaterialType type);

    private:
    Shader*mPhongShader{nullptr};
    Shader*mWhiteShader{nullptr};
};
