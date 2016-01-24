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
		AtmosphereRadius,
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
	GLint lAttributes[TotalAttributes];
	AtmosphereAttributes attributes;

	Program shaders;

	float time;

	vec2 sunPolar;
	float radius;

	vec3 sunPosition;
	GLint lSunPosition;

	GLint lModel;
	GLint lView;
	GLint lProjection;

	Model sun;
	Model skyDome;
	Model surface;
};

#endif