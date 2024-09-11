#version 460 core
out vec4 FragColor;

in vec2 uv;
in vec3 normal;//法线
in vec3 worldPosition; //片元位置
in vec3 normalv; //转职 逆矩阵后的法线

uniform sampler2D sampler;

uniform float time;

//光源参数
uniform vec3 lightDirection; //光源方向
uniform vec3 lightColor;//光源颜色
uniform vec3 lightPos; // 光源位置


uniform vec3 ambientColor; //环境光颜色

//相机参数
uniform vec3 cameraPosition;

//材质参数
uniform float spcularIntensity;
//计算漫反射光
vec3 result_diffuse()
{
	//获取当前物体的颜色
	vec3 objectColor = texture(sampler, uv).rgb;

	//准备漫反射光照
	vec3 normalN = normalize(normal);
	vec3 lightDirN = normalize(lightDirection);

	//将负数的情况清理为0 clamp函数
	float diffuse = clamp(dot(normalN, -lightDirN), 0.0, 1.0);
	//计算最终颜色
	vec3 result = diffuse * lightColor * objectColor;
	return result;
}

vec3 specular()
{
	vec3 normalN = normalize(normal);
	vec3 lightDirN = normalize(lightDirection);
	vec3 viewDirN = normalize(worldPosition - cameraPosition);


	//确认光的方向是否照射到物体上
	vec3 dotResult = reflect(-lightDirN,normalN);
	vec3 flag = step(0.0,dotResult);
	//计算反射光
	vec3 lightReflect  = normalize(reflect(lightDirN,normalN));
	//计算高光
	float spcular = max(dot(lightReflect,-viewDirN),0.0);

	//控制光板大小
	spcular = pow(spcular,64.0);
	
	vec3 spcularColor = lightColor * spcular * flag * spcularIntensity;

	return spcularColor;
}

//点光源
vec3 pointLight()
{
	//获取当前物体的颜色
	vec3 objectColor = texture(sampler, uv).rgb;

	//准备漫反射光照
	vec3 normalN = normalize(normalv);
	



	//光线方向(光源pos - 片元pos)
	// vec3 lightDirN = normalize(lightPos - worldPosition);
	vec3 lightDirN = normalize(lightPos - worldPosition);

	

	//计计算漫反射光
	float diffuse = clamp(dot(normalN,-lightDirN),0.0,1.0f);
	//计算最终颜色
	vec3 result = diffuse*lightColor*objectColor;

	//计算高光
	vec3 viewDirN = normalize( cameraPosition-worldPosition);

	//确认光的方向是否照射到物体上
	vec3 dotResult = reflect(-lightDirN,normalN);
	vec3 flag = step(0.0,dotResult);

	//计算反射光
	vec3 reflectDirN = reflect(-lightDirN,normalN);
	//Blinn-Phong模型
	float specular = max(dot(reflectDirN,-viewDirN),0.0);
	specular = pow(specular,32.0);

	// 计算最终颜色
	vec3 finalColor = specular * lightColor * spcularIntensity*flag + result + ambientColor*objectColor;
	return finalColor;
	
}
void main()
{
	//获取当前物体的颜色
	vec3 objectColor = texture(sampler, uv).rgb;
	vec3 ambientColor_ = objectColor * ambientColor;
	vec3 finalColor = result_diffuse() + specular() + ambientColor_;
	FragColor = vec4(pointLight(), 1.0);
}