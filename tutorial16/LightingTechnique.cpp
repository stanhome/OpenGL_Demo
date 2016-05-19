#include <assert.h>

#include "LightingTechnique.h"

const char *FILE_VERTEX_SHADER = "shader.vert";
const char *FILE_FRAGMENT_SHADER = "shader.frag";


GLuint m_WVPLocation;
GLuint m_samplerLocation;

LightingTechnique::LightingTechnique()
	: m_WVPLocation(0)
	, m_worldMatrixLocation(0)
	, m_samplerLocation(0)
	, m_eyeWorldPosLocation(0)
	, m_matSpecularIntensityLocation(0)
	, m_matSpecularPowerLocation(0)
{
	memset(&m_dirLightLocation, 0, sizeof(DirLightLocation));
}

bool LightingTechnique::init()
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
	m_samplerLocation = getUniformLocation("gSampler");
	m_worldMatrixLocation = getUniformLocation("gWorld");
	m_eyeWorldPosLocation = getUniformLocation("gEyeWorldPos");
	m_matSpecularIntensityLocation = getUniformLocation("gMatSpecularIntensity");
	m_matSpecularPowerLocation = getUniformLocation("gSpecularPower");

	m_dirLightLocation.Color = getUniformLocation("gDirectionalLight.Color");
	m_dirLightLocation.AmbientIntensity = getUniformLocation("gDirectionalLight.AmbientIntensity");
	m_dirLightLocation.Direction = getUniformLocation("gDirectionalLight.Direction");
	m_dirLightLocation.DiffuseIntensity = getUniformLocation("gDirectionalLight.DiffuseIntensity");


	if (m_WVPLocation == 0xFFFFFFFF ||
		m_samplerLocation == 0xFFFFFFFF ||
		m_worldMatrixLocation == 0xFFFFFFFF ||
		m_eyeWorldPosLocation == 0xFFFFFFFF ||
		m_matSpecularIntensityLocation == 0xFFFFFFFF ||
		m_matSpecularPowerLocation == 0xFFFFFFFF ||
		m_dirLightLocation.Color == 0xFFFFFFFF ||
		m_dirLightLocation.AmbientIntensity == 0xFFFFFFFF ||
		m_dirLightLocation.Direction == 0xFFFFFFFF ||
		m_dirLightLocation.DiffuseIntensity == 0xFFFFFFFF
		)
	{
		return false;
	}

	return true;
}

void LightingTechnique::setWVP(const Matrix4f &wvp)
{
	//加载数据到着色器的一致变量中
	glUniformMatrix4fv(m_WVPLocation, 1, GL_TRUE, (const GLfloat *)wvp.m);
}

void LightingTechnique::SetWorldMatrix(const Matrix4f &worldInverse)
{
	glUniformMatrix4fv(m_worldMatrixLocation, 1, GL_TRUE, (const GLfloat *)worldInverse.m);
}

void LightingTechnique::setTextureUnit(unsigned int textureUnit)
{
	glUniform1i(m_samplerLocation, textureUnit);
}

void LightingTechnique::setDirectionalLight(const DirectionalLight &light)
{
	glUniform3f(m_dirLightLocation.Color, light.color.x, light.color.y, light.color.z);
	glUniform1f(m_dirLightLocation.AmbientIntensity, light.ambientIntensity);
	Vector3f direction = light.direction;
	direction.normalize();
	glUniform3f(m_dirLightLocation.Direction, direction.x, direction.y, direction.z);
	glUniform1f(m_dirLightLocation.DiffuseIntensity, light.diffuseIntensity);
}

void LightingTechnique::setEyeWorldPos(const Vector3f &eyeWorldPos)
{
	glUniformMatrix3fv(m_eyeWorldPosLocation, 1, GL_TRUE, (const GLfloat *)eyeWorldPos);
}

void LightingTechnique::setMatSpecularIntensity(float intensity)
{
	glUniform1f(m_matSpecularIntensityLocation, intensity);
}

void LightingTechnique::setMatSpecularPower(float power)
{
	glUniform1f(m_matSpecularPowerLocation, power);
}
