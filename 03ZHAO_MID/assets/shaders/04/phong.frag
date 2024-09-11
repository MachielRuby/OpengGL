#version 460 core
out vec4 FragColor;

in vec2 uv;
in vec3 normal;
in vec3 worldPosition;

uniform sampler2D sampler;
uniform sampler2D specularMaskSampler;

//��Դ����
uniform vec3 lightPosition;
uniform vec3 targetDirection;
uniform float visibleAngle;
uniform  vec3 lightColor;


uniform vec3 ambientColor;

//�������λ��
uniform vec3 cameraPosition;

uniform float specularIntensity;

uniform float shiness;

void main()
{
	//������յ�ͨ������
	vec3 objectColor  = texture(sampler, uv).xyz;
	vec3 normalN = normalize(normal);
	vec3 lightDirN = normalize(worldPosition - lightPosition);
	vec3 viewDir = normalize(worldPosition - cameraPosition);
	vec3 targetDirN = normalize(targetDirection);

	//����spotlight�����䷶Χ
	float cTheta = dot(lightDirN, targetDirN);
	float cVisible = cos(visibleAngle);
	float spotFlag  = step(cVisible, cTheta);

	//���㷴���
	float diffuse = clamp(dot(-lightDirN, normalN), 0.0,1.0);
	vec3 diffuseColor = lightColor * diffuse * objectColor;
	
	//����specular
	//��ֹ�����Ч��
	float dotResult = dot(-lightDirN, normalN);
	float flag = step(0.0, dotResult);

	vec3 lightReflect = normalize(reflect(lightDirN,normalN));

	float specular = max(dot(lightReflect,-viewDir), 0.0);

	//���ƹ�ߴ�С
	specular = pow(specular, shiness);

	float specularMask = texture(specularMaskSampler, uv).r;

	vec3 specularColor = lightColor * specular * flag * specularIntensity*specularMask;


	//���������
	vec3 ambientColor = objectColor * ambientColor;

	vec3 finalColor = diffuseColor*spotFlag + specularColor*spotFlag + ambientColor;

	FragColor = vec4(finalColor, 1.0);
}