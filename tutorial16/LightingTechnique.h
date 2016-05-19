/* 
 * @module:
 * @brief:
 * @author: stan
 * @date:
 */

#pragma once

#include "ogl/technique.h"
#include "ogl/math_3d.h"
#include "ogl/light/lights_common.h"

class LightingTechnique : public Technique
{
private:
	struct DirLightLocation
	{
		GLuint Color;
		GLuint AmbientIntensity;
		GLuint Direction;
		GLuint DiffuseIntensity;
	};

public:
	LightingTechnique();

	virtual bool init();

	void setWVP(const Matrix4f &wvp);
	void SetWorldMatrix(const Matrix4f &worldInverse);
	void setTextureUnit(unsigned int textureUnit);
	void setDirectionalLight(const DirectionalLight &light);
	void setEyeWorldPos(const Vector3f &eyeWorldPos);
	void setMatSpecularIntensity(float intensity);
	void setMatSpecularPower(float power);

private:

	//这个句柄来获取着色器中的世界矩阵一致变量地址(WVP world-view-projection)
	GLuint m_WVPLocation;
	GLuint m_worldMatrixLocation;
	GLuint m_samplerLocation;
	GLuint m_eyeWorldPosLocation;
	GLuint m_matSpecularIntensityLocation;
	GLuint m_matSpecularPowerLocation;

	DirLightLocation m_dirLightLocation;
};