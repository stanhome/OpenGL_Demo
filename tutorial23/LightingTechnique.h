#pragma once
#include "ogl/light/BasicLightingTechnique.h"

class LightingTechnique : public BasicLightingTechnique
{
public:
	LightingTechnique();

	virtual bool init() override;

	void setLightWVP(const Matrix4f &lightWVP);
	void setShadowMapTextureUnit(unsigned int textureUnit);

private:
	GLuint m_lightWVPLocation;
	GLuint m_shadowMapLocation;
};