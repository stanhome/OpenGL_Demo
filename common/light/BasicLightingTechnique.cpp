#include <assert.h>

#include "ogl/light/BasicLightingTechnique.h"

const char *FILE_VERTEX_SHADER = "shader.vert";
const char *FILE_FRAGMENT_SHADER = "shader.frag";
//const char *FILE_VERTEX_SHADER = "lighting.vs";
//const char *FILE_FRAGMENT_SHADER = "lighting.fs";


BasicLightingTechnique::BasicLightingTechnique()
	: m_WVPLocation(0)
	, m_worldMatrixLocation(0)
	, m_samplerLocation(0)
	, m_eyeWorldPosLocation(0)
	, m_matSpecularIntensityLocation(0)
	, m_matSpecularPowerLocation(0)
	, m_numPointLightsLocation(0)
	, m_numSpotLightsLocation(0)
{
	memset(&m_dirLightLocation, 0, sizeof(DirLightLocation));
	memset(&m_pointLightsLocation, 0, sizeof(PointLightsLocation));
	memset(&m_spotLightsLocation, 0, sizeof(SpotLightsLocation));
}

bool BasicLightingTechnique::init()
{
	if (!Technique::init())
	{
		return false;
	}

	if (!addShader(GL_VERTEX_SHADER, FILE_VERTEX_SHADER))
	{
		fprintf(stderr, "[e]added shader failed:%s\n", FILE_VERTEX_SHADER);
		return false;
	}

	if (!addShader(GL_FRAGMENT_SHADER, FILE_FRAGMENT_SHADER))
	{
		fprintf(stderr, "[e]added shader failed:%s\n", FILE_FRAGMENT_SHADER);
		return false;
	}

	if (!finalize())
	{
		return false;
	}

	m_WVPLocation = getUniformLocation("gWVP");
	m_worldMatrixLocation = getUniformLocation("gWorld");
	m_samplerLocation = getUniformLocation("gSampler");
	m_eyeWorldPosLocation = getUniformLocation("gEyeWorldPos");

	m_dirLightLocation.Color = getUniformLocation("gDirectionalLight.Base.Color");
	m_dirLightLocation.AmbientIntensity = getUniformLocation("gDirectionalLight.Base.AmbientIntensity");
	m_dirLightLocation.Direction = getUniformLocation("gDirectionalLight.Direction");
	m_dirLightLocation.DiffuseIntensity = getUniformLocation("gDirectionalLight.Base.DiffuseIntensity");

	m_matSpecularIntensityLocation = getUniformLocation("gMatSpecularIntensity");
	m_matSpecularPowerLocation = getUniformLocation("gSpecularPower");
	m_numPointLightsLocation = getUniformLocation("gNumPointLights");
	m_numSpotLightsLocation = getUniformLocation("gNumSpotLights");

	//point lights location
	for (unsigned int i = 0; i < ARRAY_LEN(m_pointLightsLocation); ++i)
	{
		PointLightsLocation &rpll = m_pointLightsLocation[i];

		char name[128] = { 0 };
		SNPRINTF(name, sizeof(name), "gPointLights[%d].Base.Color", i);
		rpll.Color = getUniformLocation(name);

		SNPRINTF(name, sizeof(name), "gPointLights[%d].Base.AmbientIntensity", i);
		rpll.AmbientIntensity = getUniformLocation(name);

		SNPRINTF(name, sizeof(name), "gPointLights[%d].Position", i);
		rpll.Position = getUniformLocation(name);

		SNPRINTF(name, sizeof(name), "gPointLights[%d].Base.DiffuseIntensity", i);
		rpll.DiffuseIntensity = getUniformLocation(name);

		SNPRINTF(name, sizeof(name), "gPointLights[%d].Atten.Constant", i);
		rpll.Atten.Constant = getUniformLocation(name);

		SNPRINTF(name, sizeof(name), "gPointLights[%d].Atten.Linear", i);
		rpll.Atten.Linear = getUniformLocation(name);

		SNPRINTF(name, sizeof(name), "gPointLights[%d].Atten.Exp", i);
		rpll.Atten.Exp = getUniformLocation(name);
	}

	//spot lights location
	for (unsigned int i = 0; i < ARRAY_LEN(m_spotLightsLocation); ++i)
	{
		SpotLightsLocation &rsll = m_spotLightsLocation[i];

		char name[128] = { 0 };
		SNPRINTF(name, sizeof(name), "gSpotLights[%d].Base.Base.Color", i);
		rsll.Color = getUniformLocation(name);

		SNPRINTF(name, sizeof(name), "gSpotLights[%d].Base.Base.AmbientIntensity", i);
		rsll.AmbientIntensity = getUniformLocation(name);

		SNPRINTF(name, sizeof(name), "gSpotLights[%d].Base.Base.DiffuseIntensity", i);
		rsll.DiffuseIntensity = getUniformLocation(name);
		
		SNPRINTF(name, sizeof(name), "gSpotLights[%d].Base.Position", i);
		rsll.Position = getUniformLocation(name);

		SNPRINTF(name, sizeof(name), "gSpotLights[%d].Base.Atten.Constant", i);
		rsll.Atten.Constant = getUniformLocation(name);

		SNPRINTF(name, sizeof(name), "gSpotLights[%d].Base.Atten.Linear", i);
		rsll.Atten.Linear = getUniformLocation(name);

		SNPRINTF(name, sizeof(name), "gSpotLights[%d].Base.Atten.Exp", i);
		rsll.Atten.Exp = getUniformLocation(name);

		SNPRINTF(name, sizeof(name), "gSpotLights[%d].Direction", i);
		rsll.Direction = getUniformLocation(name);

		SNPRINTF(name, sizeof(name), "gSpotLights[%d].Cutoff", i);
		rsll.Cutoff = getUniformLocation(name);
	}

	return true;
}

void BasicLightingTechnique::setWVP(const Matrix4f &wvp)
{
	//加载数据到着色器的一致变量中
	glUniformMatrix4fv(m_WVPLocation, 1, GL_TRUE, (const GLfloat *)wvp.m);
}

void BasicLightingTechnique::SetWorldMatrix(const Matrix4f &worldInverse)
{
	glUniformMatrix4fv(m_worldMatrixLocation, 1, GL_TRUE, (const GLfloat *)worldInverse.m);
}

void BasicLightingTechnique::setTextureUnit(unsigned int textureUnit)
{
	glUniform1i(m_samplerLocation, textureUnit);
}

void BasicLightingTechnique::setDirectionalLight(const DirectionalLight &light)
{
	glUniform3f(m_dirLightLocation.Color, light.color.x, light.color.y, light.color.z);
	glUniform1f(m_dirLightLocation.AmbientIntensity, light.ambientIntensity);
	Vector3f direction = light.direction;
	direction.normalize();
	glUniform3f(m_dirLightLocation.Direction, direction.x, direction.y, direction.z);
	glUniform1f(m_dirLightLocation.DiffuseIntensity, light.diffuseIntensity);
}

void BasicLightingTechnique::setPointLights(unsigned int numLights, const PointLight *pLights)
{
	glUniform1i(m_numPointLightsLocation, numLights);
	for (unsigned int i = 0; i < numLights; ++i)
	{
		PointLightsLocation &rpll = m_pointLightsLocation[i];
		const PointLight &rpl = pLights[i];
		glUniform3f(rpll.Color, rpl.color.x, rpl.color.y, rpl.color.z);
		glUniform1f(rpll.AmbientIntensity, rpl.ambientIntensity);
		glUniform1f(rpll.DiffuseIntensity, rpl.diffuseIntensity);
		glUniform3f(rpll.Position, rpl.position.x, rpl.position.y, rpl.position.z);
		glUniform1f(rpll.Atten.Constant, rpl.attenuation.constant);
		glUniform1f(rpll.Atten.Linear, rpl.attenuation.linear);
		glUniform1f(rpll.Atten.Exp, rpl.attenuation.exp);
	}
}

void BasicLightingTechnique::setSpotLights(unsigned int numLights, const SpotLight *pLights)
{
	glUniform1i(m_numSpotLightsLocation, numLights);

	for (unsigned int i = 0; i < numLights; ++i)
	{
		SpotLightsLocation &rsll = m_spotLightsLocation[i];
		const SpotLight &rsl = pLights[i];
		glUniform3f(rsll.Color, rsl.color.x, rsl.color.y, rsl.color.z);
		glUniform1f(rsll.AmbientIntensity, rsl.ambientIntensity);
		glUniform1f(rsll.DiffuseIntensity, rsl.diffuseIntensity);
		glUniform3f(rsll.Position, rsl.position.x, rsl.position.y, rsl.position.z);
		glUniform1f(rsll.Atten.Constant, rsl.attenuation.constant);
		glUniform1f(rsll.Atten.Linear, rsl.attenuation.linear);
		glUniform1f(rsll.Atten.Exp, rsl.attenuation.exp);
		Vector3f direction = rsl.direction;
		direction.normalize();
		glUniform3f(rsll.Direction, direction.x, direction.y, direction.z);
		glUniform1f(rsll.Cutoff, cosf(toRadian(rsl.cutoff)));
	}
}

void BasicLightingTechnique::setEyeWorldPos(const Vector3f &eyeWorldPos)
{
	glUniformMatrix3fv(m_eyeWorldPosLocation, 1, GL_TRUE, (const GLfloat *)eyeWorldPos);
}

void BasicLightingTechnique::setMatSpecularIntensity(float intensity)
{
	glUniform1f(m_matSpecularIntensityLocation, intensity);
}

void BasicLightingTechnique::setMatSpecularPower(float power)
{
	glUniform1f(m_matSpecularPowerLocation, power);
}
