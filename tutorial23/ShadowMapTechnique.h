/* 
 * @module:
 * @brief:
 * @author: stan
 * @date:
 */

#pragma once

#include "ogl/technique.h"
#include "ogl/math_3d.h"

class ShadowMapTechnique : public Technique
{
public:
	ShadowMapTechnique();

	virtual bool init() override;

	void setWVP(const Matrix4f &wvp);
	void setTextureUnit(unsigned int textureUnit);
private:
	GLuint m_wvpLocation;
	GLuint m_textureLocation;
};