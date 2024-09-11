#version 460 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aUV;
layout (location = 2) in vec3 aNormal;

out vec2 uv;
out vec3 normal;
//计算世界坐标
out vec3 worldPosition;
out vec3 normalv;

uniform mat4 modelTransform;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

//aPos作为attribute（属性）传入shader
//不允许更改的
void main()
{
	//计算世界坐标
	vec4 transformPosition = vec4(aPos, 1.0);
	//做一个中间变量TransformPosition，用于计算四位位置与modelTransform相乘的中间结果
	transformPosition = modelTransform * transformPosition;
	worldPosition = transformPosition.xyz;
	gl_Position = projectionMatrix*viewMatrix*transformPosition;

	uv = aUV;
	normal = aNormal;
	  // 计算并传递变换到世界空间的法线
    normalv = mat3(transpose(inverse(modelTransform))) * aNormal;
}