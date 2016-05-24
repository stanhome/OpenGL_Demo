#include "ShadowMapTechnique.h"
#include "ogl/util.h"

ShadowMapTechnique::ShadowMapTechnique()
	: m_wvpLocation(0)
	, m_textureLocation(0)
{

}

bool ShadowMapTechnique::init()
{
	if (!Technique::init())
	{
		return false;
	}

	if (!addShader(GL_FRAGMENT_SHADER, "shadow_map.frag"))
	{
		return false;
	}

	if (!addShader(GL_VERTEX_SHADER, "shadow_map.vert"))
	{
		return false;
	}

	if (!finalize())
	{
		return false;
	}

	m_wvpLocation = getUniformLocation("gWVP");
	m_textureLocation = getUniformLocation("gShadowMap");
	if (m_wvpLocation == INVALID_UNIFORM_LOCATION ||
		m_textureLocation == INVALID_UNIFORM_LOCATION)
	{
		return false;
	}

	return true;
}

void ShadowMapTechnique::setWVP(const Matrix4f &wvp)
{
	glUniformMatrix4fv(m_wvpLocation, 1, GL_FLOAT, (const GLfloat *)wvp.m);
}

void ShadowMapTechnique::setTextureUnit(unsigned int textureUnit)
{
	glUniform1i(m_textureLocation, textureUnit);
}