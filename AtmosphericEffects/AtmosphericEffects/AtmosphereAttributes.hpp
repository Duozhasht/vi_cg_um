#ifndef ATMOSPHERE_ATTRIBUTES_HPP
#define ATMOSPHERE_ATTRIBUTES_HPP

#include "Types.hpp"

struct AtmosphereAttributes
{
	AtmosphereAttributes(float pr = 6371e3, float ar = 6471e3, vec3 rlh = vec3(5.5e-6, 13.0e-6, 22.4e-6), float mie = 21e-6, float sunI = 20.f, float rlhSh = 8e3, float mieSh = 1.2e3, float g = 0.78);

	bool load(const char *json);

	float PlanetRadius;
	float AtmosphereRadius;
	vec3  RayleighCoefs;
	float MieCoef;
	float SunIntensity;
	float RayleighScaleH;
	float MieScaleH;
	float G;

private:
	void calculateRayleigh();

	float n;
	float N;
};

#endif
