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

class Renderer
{
    public:
    Renderer();
    ~Renderer();

    //每次调用都会熏染
    void render(
        const std::vector<Mesh*>& meshes,
        Camera*camera,
        PointLight*pointLight,
        DirectionalLight*dirLight,
        SpotLight*spotLight,
        AmbientLight*ambLight
    );
    private:
    Shader*pickShader(MaterialType type);

    private:
    Shader*mPhongShader{nullptr};
    Shader*mWhiteShader{nullptr};
};
