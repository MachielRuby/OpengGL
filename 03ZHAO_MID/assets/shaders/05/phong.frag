#version 460 core
out vec4 FragColor;

in vec2 uv;
in vec3 normal;
in vec3 worldPosition;

uniform sampler2D sampler;	//diffuse贴图采样器
uniform sampler2D specularMaskSampler;//specularMask贴图采样器

//光源参数
uniform vec3 lightPosition;
uniform vec3 targetDirection;

//可以将inner跟outer两个角度的cosine值在cpu计算完毕，传入shader
uniform float innerLine;//cosθ
uniform float outerLine;//cosβ

uniform vec3 lightColor;

uniform vec3 ambientColor;

//相机世界位置
uniform vec3 cameraPosition;

uniform float specularIntensity;

uniform float shiness;

void main()
{
	//计算光照的通用数据
	vec3 objectColor  = texture(sampler, uv).xyz;
	vec3 normalN = normalize(normal);
	vec3 lightDirN = normalize(worldPosition - lightPosition);
	vec3 viewDir = normalize(worldPosition - cameraPosition);
	vec3 targetDirN = normalize(targetDirection);

	//计算spotlight的照射范围
	float cGamma = dot(lightDirN, targetDirN);
	float spotIntensity =clamp( (cGamma - outerLine) / (innerLine - outerLine), 0.0, 1.0);

	//2 计算diffuse
	float diffuse = clamp(dot(-lightDirN, normalN), 0.0,1.0);
	vec3 diffuseColor = lightColor * diffuse * objectColor;
	

	//计算specular
	//防止背面光效果
	float dotResult = dot(-lightDirN, normalN);
	float flag = step(0.0, dotResult);

	vec3 lightReflect = normalize(reflect(lightDirN,normalN));

	float specular = max(dot(lightReflect,-viewDir), 0.0);

	//控制光斑大小
	specular = pow(specular, shiness);

	float specularMask = texture(specularMaskSampler, uv).r;

	vec3 specularColor = lightColor * specular * flag * specularIntensity * specularMask;


	//环境光计算
	vec3 ambientColor = objectColor * ambientColor;


	vec3 finalColor = (diffuseColor + specularColor) * spotIntensity + ambientColor;

	FragColor = vec4(finalColor, 1.0);
}