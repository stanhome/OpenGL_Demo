/* 
 * @module:
 * @brief:
 * @author: stan
 * @date:
 */

#pragma once

#include <AntTweakBar.h>

#include "ogl/math_3d.h"

class BaseLight
{
public:
	std::string name;
	Vector3f color;
	float ambientIntensity;
	float diffuseIntensity;

	BaseLight()
		: name()
		, color(0.0f, 0.0f, 0.0f)
		, ambientIntensity(0.0f)
		, diffuseIntensity(0.0f)
	{
	}

	virtual void addToATB(TwBar *bar);
};


class DirectionalLight : public BaseLight
{
public:
	Vector3f direction;

	DirectionalLight()
		: direction(0.0f, 0.0f, 0.0f)
	{
	}

	virtual void addToATB(TwBar *bar) override;
};

//点光源衰减公式中的3个系数
struct LightAttenuation
{
	float constant;
	float linear;
	float exp;
};


class PointLight : public BaseLight
{
public:
	Vector3f position;
	LightAttenuation attenuation;

	PointLight()
		: position(0.0f)
		, attenuation()
	{
		attenuation.constant = 0.0f;
		attenuation.linear = 0.0f;
		attenuation.exp = 0.0f;
	}

	virtual void addToATB(TwBar *bar) override;
};


class SpotLight : public PointLight
{
public:
	Vector3f direction;
	//聚光灯范围的临界值
	float cutoff;
	SpotLight() 
		: direction(0.0f), cutoff(0.0f)
	{
	}

	virtual void addToATB(TwBar *bar) override;
};