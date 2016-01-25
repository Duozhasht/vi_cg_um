#ifndef ATMOSPHERE_HPP
#define ATMOSPHERE_HPP

#include "Program.hpp"
#include "Model.hpp"
#include "AtmosphereAttributes.hpp"

class Atmosphere
{
private:
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
	void onResize(int newWidth, int newHeight);
	void onUpdate(float t);

	void setTime(float t) { time = t; }
	vec3 getSunPosition() { return sunPosition; }

	void draw(mat4 &view, mat4& projection);
	void onDelete();

private:
	void setUniformAttributes();

	GLint lAttributes[TotalAttributes];
	AtmosphereAttributes attributes;

	Program skyShaders;
	Program groundShaders;

	float time;

	vec2 sunPolar;
	float radius;

	vec3 sunPosition;
	GLint lSunPosition;

	GLint lModelS, lModelG;
	GLint lViewS, lViewG;
	GLint lProjectionS, lProjectionG;

	GLuint texture;

	Model sun;
	Model skyDome;
	Model surface;
};

#endif