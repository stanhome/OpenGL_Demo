/**
 * created by stan
 */

#include "LightingTechnique.h"

LightingTechnique::LightingTechnique()
	: m_lightWVPLocation(0)
	, m_shadowMapLocation(0)
{

}

bool LightingTechnique::init()
{
	if (!BasicLightingTechnique::init())
	{
		return false;
	}

	m_lightWVPLocation = getUniformLocation("gLightWVP");
	m_shadowMapLocation = getUniformLocation("gShadowMap");

	if (m_lightWVPLocation == INVALID_UNIFORM_LOCATION || m_shadowMapLocation == INVALID_UNIFORM_LOCATION)
	{
		return false;
	}

	return true;
}

void LightingTechnique::setLightWVP(const Matrix4f &lightWVP)
{
	glUniformMatrix4fv(m_lightWVPLocation, 1, GL_TRUE, (const GLfloat *)lightWVP.m);
}

void LightingTechnique::setShadowMapTextureUnit(unsigned int textureUnit)
{
	glUniform1i(m_shadowMapLocation, textureUnit);
}