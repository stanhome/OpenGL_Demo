#version 330

in vec2 TexCoord0;
in vec3 Normal0;
in vec3 WorldPos0;

out vec4 FragColor;

struct DirectionalLight
{
	vec3 Color;
	float AmbientIntensity;
	float DiffuseIntensity;
	vec3 Direction;
};

uniform DirectionalLight gDirectionalLight;
uniform sampler2D gSampler;
uniform vec3 gEyeWorldPos;
uniform float gMatSpecularIntensity;
uniform float gSpecularPower;

void main()
{
	vec4 ambientColor = vec4(gDirectionalLight.Color, 1.0f) * gDirectionalLight.AmbientIntensity;
	vec3 lightDirection = -gDirectionalLight.Direction;
	vec3 normal = normalize(Normal0);
	float diffuseFactor = dot(normal, lightDirection);
	vec4 diffuseColor = vec4(0, 0, 0, 0);
	vec4 specularColor = vec4(0, 0, 0, 0);
	
	if (diffuseFactor > 0) {
		diffuseColor = vec4(gDirectionalLight.Color, 1.0f) * gDirectionalLight.DiffuseIntensity * diffuseFactor;
		vec3 vertexToEye = normalize(gEyeWorldPos - WorldPos0);
		vec3 lightReflect = normalize(reflect(gDirectionalLight.Direction, normal));
		float specularFactor = dot(vertexToEye, lightReflect);
		specularFactor = pow(specularFactor, gSpecularPower);

		if (specularFactor > 0) {
			specularColor = vec4(gDirectionalLight.Color, 1.0f) * gMatSpecularIntensity * specularFactor;
		}
	}

	FragColor = texture2D(gSampler, TexCoord0.xy) * (ambientColor + diffuseColor + specularColor);
}