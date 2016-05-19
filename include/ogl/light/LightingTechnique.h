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
#include "ogl/util.h"

class LightingTechnique : public Technique
{
private:
	struct BaseLightLocation
	{
		GLuint Color;
		GLuint AmbientIntensity;
		GLuint DiffuseIntensity;
	};

	struct DirLightLocation : public BaseLightLocation
	{
		GLuint Direction;
	};

	struct PointLightsLocation : public BaseLightLocation
	{
		GLuint Position;
		//衰减系数
		struct
		{
			GLuint Constant;
			GLuint Linear;
			GLuint Exp;
		} Atten;
	};

	struct SpotLightsLocation : public PointLightsLocation
	{
		GLuint Direction;
		GLuint Cutoff;
	};


public:
	static const unsigned int MAX_POINT_LIGHTS = 2;
	static const unsigned int MAX_SPOT_LIGHTS = 2;

	LightingTechnique();

	virtual bool init();

	void setWVP(const Matrix4f &wvp);
	void SetWorldMatrix(const Matrix4f &worldInverse);
	void setTextureUnit(unsigned int textureUnit);
	void setDirectionalLight(const DirectionalLight &light);
	void setPointLights(unsigned int numLights, const PointLight *pLights);
	void setSpotLights(unsigned int numLights, const SpotLight *pLights);
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
	GLuint m_numPointLightsLocation;
	GLuint m_numSpotLightsLocation;

	DirLightLocation m_dirLightLocation;
	PointLightsLocation m_pointLightsLocation[MAX_POINT_LIGHTS];
	SpotLightsLocation m_spotLightsLocation[MAX_SPOT_LIGHTS];
};