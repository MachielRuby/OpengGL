#version 460 core
out vec4 FragColor;

in vec2 uv;
in vec3 normal;
in vec3 worldPosition;

uniform sampler2D sampler;
uniform sampler2D specularMaskSampler; //镜面反射遮罩

uniform vec3 ambientColor;

//相机世界位置
uniform vec3 cameraPosition;

uniform float shiness;

struct DirectionalLight //平行光
{
    vec3 direction;
    vec3 color;
    float specularIntensity; //镜面反射强度
};

struct PointLight //点光源
{
    vec3 position;
    vec3 color;
    float specularIntensity; //

    float k2;
    float k1;
    float k0;
};

struct SpotLight //聚光灯
{
    vec3 position;
    vec3 targetDirection;
    vec3 color;
    float outerLine;
    float innerLine;
    float specularIntensity;
};

uniform SpotLight spotLight;
uniform DirectionalLight directionalLight;
uniform PointLight pointLight;

//计算漫反射
vec3 calculateDiffuse(vec3 lightColor,vec3 objectColor,vec3 lightDir,vec3 normal)
{
    float diffuse = clamp(dot(-lightDir,normal),0.0,1.0);
    vec3 diffuseColor = diffuse * lightColor *objectColor;//漫反射颜色
    return diffuseColor;
}

//计算镜面反射
vec3 calculateSpecular(vec3 lightColor,vec3 lightDir,vec3 normal,vec3 viewDir,float intensity)
{
    //防止背面光效果
    float doResult = dot(-lightDir,normal);
    float flag = step(0.0,doResult);
    vec3 lightReflect = normalize(reflect(lightDir,normal));

    //计算specular
    float specular  = max(dot(lightReflect,-viewDir),0.0f);
    specular = pow(specular,shiness);

    //float specularMask = texture(specularMaskSampler,uv).r;
    vec3 specularColor = flag * specular * lightColor * intensity;//镜面反射颜色
    return specularColor;
}
//计算聚光灯
vec3 calculateSpotLight(SpotLight light,vec3 normal,vec3 viewDir)
{
    //计算光照的通用数据
    vec3 objectColor = texture(sampler,uv).rgb;
    vec3 lightDir = normalize(worldPosition - light.position);
    vec3 targetDir = normalize(light.targetDirection);

    //计算spotlight照射范围
    float cGamma = dot(lightDir,targetDir);
    float intensity = clamp((cGamma - light.outerLine)/(light.innerLine - light.outerLine),0.0,1.0);

    //计算diffuse
    vec3 diffuseColor = calculateDiffuse(light.color,objectColor,lightDir,normal);

    //计算specular
    vec3 specularColor = calculateSpecular(light.color,lightDir,normal,viewDir,light.specularIntensity);

    //计算最终颜色
    return (diffuseColor+specularColor)*intensity;    
}

//计算环境光
vec3 calculateDirectionalLight(DirectionalLight light, vec3 normal ,vec3 viewDir){
	//计算光照的通用数据
	vec3 objectColor  = texture(sampler, uv).xyz;
	vec3 lightDir = normalize(light.direction);

	//1 计算diffuse
	vec3 diffuseColor = calculateDiffuse(light.color,objectColor, lightDir,normal);

	//2 计算specular
	vec3 specularColor = calculateSpecular(light.color, lightDir,normal, viewDir,light.specularIntensity); 

	return diffuseColor + specularColor;
}
//计算点光源
vec3 calculatePointLight(PointLight light, vec3 normal ,vec3 viewDir){
	//计算光照的通用数据
	vec3 objectColor  = texture(sampler, uv).xyz;
	vec3 lightDir = normalize(worldPosition - light.position);

	//计算衰减
	float dist = length(worldPosition - light.position);
	float attenuation = 1.0 / (light.k2 * dist * dist + light.k1 * dist + light.k0);

	//1 计算diffuse
	vec3 diffuseColor = calculateDiffuse(light.color,objectColor, lightDir,normal);

	//2 计算specular
	vec3 specularColor = calculateSpecular(light.color, lightDir,normal, viewDir,light.specularIntensity); 

	return (diffuseColor + specularColor)*attenuation;
}
void main()
{
    vec3 result  = vec3(0.0,0.0,0.0);

    //计算光照的通用数据
    vec3 objectColor = texture(sampler,uv).rgb;
    vec3 normalN = normalize(normal);
    vec3 lightDirN  = normalize(worldPosition - spotLight.position);
    vec3 viewDir = normalize(worldPosition - cameraPosition);
    vec3 targetDirN = normalize(spotLight.targetDirection);

    result += calculateSpotLight(spotLight,normalN,viewDir);
    result += calculateDirectionalLight(directionalLight,normalN, viewDir);
	result += calculatePointLight(pointLight,normalN, viewDir);

    vec3 ambientColor = objectColor * ambientColor;

    vec3 finalColor = result + ambientColor;
    FragColor = vec4(finalColor,1.0);
}