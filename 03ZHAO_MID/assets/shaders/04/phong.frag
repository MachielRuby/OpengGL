#version 460 core
out vec4 FragColor;

in vec2 uv;
in vec3 normal;
in vec3 worldPosition;

uniform sampler2D sampler;
uniform sampler2D specularMaskSampler;

//光源参数
uniform vec3 lightPosition;
uniform vec3 targetDirection;
uniform float visibleAngle;
uniform  vec3 lightColor;


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
	float cTheta = dot(lightDirN, targetDirN);
	float cVisible = cos(visibleAngle);
	float spotFlag  = step(cVisible, cTheta);

	//计算反射光
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

	vec3 specularColor = lightColor * specular * flag * specularIntensity*specularMask;


	//环境光计算
	vec3 ambientColor = objectColor * ambientColor;

	vec3 finalColor = diffuseColor*spotFlag + specularColor*spotFlag + ambientColor;

	FragColor = vec4(finalColor, 1.0);
}