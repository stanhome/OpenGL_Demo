#version 330

const int MAX_POINT_LIGHTS = 2;
const int MAX_SPOT_LIGHTS = 2;

in vec2 TexCoord0;
in vec3 Normal0;
in vec3 WorldPos0;

out vec4 FragColor;

struct BaseLight
{
	vec3 Color;
	float AmbientIntensity;
	float DiffuseIntensity;
};

struct DirectionalLight
{
	BaseLight Base;
	vec3 Direction;
};

struct Attenuation
{
	float Constant;
	float Linear;
	float Exp;
};

struct PointLight
{
	BaseLight Base;
	vec3 Position;
	Attenuation Atten;
};

struct SpotLight
{
	PointLight Base;
	vec3 Direction;
	float Cutoff;
};

uniform int gNumPointLights;
uniform int gNumSpotLights;
uniform DirectionalLight gDirectionalLight;
uniform PointLight gPointLights[MAX_POINT_LIGHTS];
uniform SpotLight gSpotLights[MAX_SPOT_LIGHTS];
uniform sampler2D gSampler;
uniform vec3 gEyeWorldPos;
uniform float gMatSpecularIntensity;
uniform float gSpecularPower;


vec4 calcLightInternal(BaseLight light, vec3 lightDirection, vec3 normal)
{
	vec4 ambientColor = vec4(light.Color * light.AmbientIntensity, 1.0f);
	float diffuseFactor = dot(normal, -lightDirection);

	vec4 diffuseColor = vec4(0, 0, 0, 0);
	vec4 specularColor = vec4(0, 0, 0, 0);

	if (diffuseFactor > 0) {
		diffuseColor = vec4(light.Color * light.DiffuseIntensity * diffuseFactor, 1.0f);

		vec3 vertexToEye = normalize(gEyeWorldPos - WorldPos0);
		vec3 lightReflect = normalize(reflect(lightDirection, normal));
		float specularFactor = dot(vertexToEye, lightReflect);
		if (specularFactor > 0) {
			specularFactor = pow(specularFactor, gSpecularPower);
			specularColor = vec4(light.Color * gMatSpecularIntensity * specularFactor, 1.0f);
		}
	}

	return (ambientColor + diffuseColor + specularColor);
}

vec4 calcDirectionalLight(vec3 normal)
{
	return calcLightInternal(gDirectionalLight.Base, gDirectionalLight.Direction, normal);
}

vec4 calcPointLight(struct PointLight light, vec3 normal)
{
	vec3 lightDirection = WorldPos0 - light.Position;
	float lightDistance = length(lightDirection);
	lightDirection = normalize(lightDirection);

	vec4 color = calcLightInternal(light.Base, lightDirection, normal);
	float attenuation = light.Atten.Constant + 
						light.Atten.Linear * lightDistance + 
						light.Atten.Exp * lightDistance * lightDistance;

	return color / attenuation;
}

vec4 calcSpotLight(struct SpotLight l, vec3 normal)
{
	vec3 lightToPixel = normalize(WorldPos0 - l.Base.Position);
	float spotFactor = dot(lightToPixel, l.Direction);
	if (spotFactor > l.Cutoff) {
		vec4 color = calcPointLight(l.Base, normal);
		return color * (1.0 - (1.0 - spotFactor) * 1.0/(1.0 - l.Cutoff));
	} else {
		return vec4(0, 0, 0, 0);
	}
}


void main()
{
	vec3 normal = normalize(Normal0);
	vec4 totalLight = calcDirectionalLight(normal);
	
	for (int i = 0; i < gNumPointLights; ++i) {
		totalLight += calcPointLight(gPointLights[i], normal);
	}

	for (int i = 0; i < gNumSpotLights; ++i) {
		totalLight += calcSpotLight(gSpotLights[i], normal);
	}

	FragColor = texture2D(gSampler, TexCoord0.xy) * totalLight;
}