#ifndef ATMOSPHERE_ATTRIBUTES_HPP
#define ATMOSPHERE_ATTRIBUTES_HPP

#include "Types.hpp"
#include "rapidjson\rapidjson.h"

struct AtmosphereAttributes
{
	AtmosphereAttributes(float pr = , float ar, vec3 rlh, float mie, float sunI, vec3 sunPos, float rlhSh, float mieSh, float g);

	float PlanetRadius;
	float AtmosphereRadius;
	vec3 RayleighCoefs;
	float MieCoef;
	float SunIntensity;
	vec3 SunPosition;
	float RayleighScaleH;
	float MieScaleH;
	float G;
};

#endif
