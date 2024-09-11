#version 460 core
out vec4 FragColor;

in vec2 uv;
in vec3 normal;
in vec3 worldPosition;

uniform sampler2D sampler;
uniform sampler2D specularMaskSampler;

//光源参数
uniform vec3 lightPosition;
uniform vec3 lightColor;
uniform float k2;
uniform float k1;
uniform float k0;


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


	//计算衰减
	float distance = length(worldPosition - lightPosition);
	float attenuation  = 1.0 / (k0 + k1 * distance + k2 * distance * distance);

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

	vec3 specularColor = lightColor * specular * flag * specularIntensity*specularMask*10.0f;


	//环境光计算
	vec3 ambientColor = objectColor * ambientColor;

	vec3 finalColor = diffuseColor*attenuation + specularColor*attenuation + ambientColor;

	FragColor = vec4(finalColor, 1.0);
}