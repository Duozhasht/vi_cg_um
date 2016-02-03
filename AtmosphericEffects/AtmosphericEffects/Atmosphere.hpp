#ifndef ATMOSPHERE_HPP
#define ATMOSPHERE_HPP

#include "Program.hpp"
#include "Model.hpp"
#include "AtmosphereAttributes.hpp"

class Atmosphere
{
public:
	enum Attribute
	{
		PlanetRadius,
		AtmosphereHeight,
		RayleighCoefs,
		MieCoef,
		SunIntensity,
		RayleighScaleH,
		MieScaleH,
		G,
		TotalAttributes
	};

	static const char* AttributeNames[TotalAttributes];

public:
	Atmosphere();

	bool create(float radius);
	bool loadAttributes(const char *file);
	void onUpdate(float t);

	vec3 getSunPosition() { return sunPosition; }

	void draw(mat4 &view, mat4& projection);

private:
	void setUniformAttributes();

	GLint lAttributes[TotalAttributes];
	AtmosphereAttributes attributes;

	Program skyShaders;
	Program groundShaders;

	vec2 sunPolar;
	float radius;

	vec3 sunPosition;
	GLint lSunPositionS;
	GLint lSunPositionG;

	GLint lModelS, lModelG;
	GLint lViewS, lViewG;
	GLint lProjectionS, lProjectionG;

	GLuint surfaceTex;
	GLuint starsTex;

	Model sun;
	Model skyDome;
	Model surface;
};

#endif