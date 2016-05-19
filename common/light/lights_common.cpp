#include "ogl/light/lights_common.h"

void BaseLight::addToATB(TwBar *bar)
{
	std::string s = name + ".Color";
	TwAddVarRW(bar, s.c_str(), TW_TYPE_COLOR3F, &color, NULL);

	s = name + ".Ambient Intensity";
	TwAddVarRW(bar, s.c_str(), TW_TYPE_FLOAT, &ambientIntensity, "min=0.0, max=1.0, step=0.005");

	s = name + ".Diffuse Intensity";
	TwAddVarRW(bar, s.c_str(), TW_TYPE_FLOAT, &diffuseIntensity, "min=0.0, max=1.0, step=0.005");
}


void DirectionalLight::addToATB(TwBar *bar)
{
	BaseLight::addToATB(bar);
	std::string s = name + ".Direction";
	TwAddVarRW(bar, s.c_str(), TW_TYPE_DIR3F, &direction, "axisz=-z");
}


void PointLight::addToATB(TwBar *bar)
{
	BaseLight::addToATB(bar);
	std::string s = name + ".Position";
	//TwAddVarRW(bar, s.c_str(), TW_VERSION)
	//TODO atb
}

void SpotLight::addToATB(TwBar *bar)
{
	PointLight::addToATB(bar);
}
