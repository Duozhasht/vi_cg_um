#include "Atmosphere.hpp"
#include <iostream>

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/polar_coordinates.hpp>

const char* Atmosphere::AttributeNames[] = { "uPlanetRadius", "uAtmosRadius", "uRhlCoefs", "uMieCoef", "uSunInt", "uRlhScaleH", "uMieScaleH", "uG" };

Atmosphere::Atmosphere()
{
	
}

bool Atmosphere::create(float r)
{
	shaders.create();

	Shader vert, frag;

	if (!vert.loadFromFile("shaders/atmosphere.vert", Shader::Vertex))
		return false;

	if (!frag.loadFromFile("shaders/atmosphere.frag", Shader::Fragment))
		return false;

	shaders.attachShader(vert);
	shaders.attachShader(frag);

	if(!shaders.compile())
		return false;

	if (!attributes.load("config/atts.json"))
		return false;

	for (int i = PlanetRadius; i < TotalAttributes; ++i)
	{
		lAttributes[i] = shaders.getUniformLocation(AttributeNames[i]);
	}

	glUseProgram(shaders.id);

	glUniform1f(lAttributes[PlanetRadius], attributes.PlanetRadius);
	glUniform1f(lAttributes[AtmosphereRadius], attributes.AtmosphereRadius);
	glUniform3f(lAttributes[RayleighCoefs], attributes.RayleighCoefs.r, attributes.RayleighCoefs.g, attributes.RayleighCoefs.b);

	glUniform1f(lAttributes[MieCoef], attributes.MieCoef);
	glUniform1f(lAttributes[SunIntensity], attributes.SunIntensity);

	glUniform1f(lAttributes[RayleighScaleH], attributes.RayleighScaleH);
	glUniform1f(lAttributes[MieScaleH], attributes.MieScaleH);
	glUniform1f(lAttributes[G], attributes.G);

	glUseProgram(0);

	lSunPosition = shaders.getUniformLocation("uSunPos");

	lModel = shaders.getUniformLocation("model");
	lView = shaders.getUniformLocation("view");
	lProjection = shaders.getUniformLocation("projection");

	sunPolar = vec2(0.0f, 0.0f);
	radius = r;

	skyDome = Model::dome(radius, 50, 50);
	surface = Model::plane(2 * radius, 50.f);

	sun = Model::sphere(1, 10.f, 10.f);

	return true;
}

void Atmosphere::onUpdate(float t)
{
	sunPolar.x += 0.01f * t;
	sunPosition = 1.5f * radius * glm::euclidean(sunPolar);
	sun.setTransform(glm::translate(sunPosition));
}

void Atmosphere::draw(mat4 &view, mat4& projection)
{
	glUseProgram(shaders.id);

	glUniformMatrix4fv(lView, 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(lProjection, 1, GL_FALSE, glm::value_ptr(projection));
	
	glUniform3f(lSunPosition, sunPosition.x, sunPosition.y, sunPosition.z);

	skyDome.draw(lModel);
	//sun.draw(lModel);

	// surface.draw(lModel);

	glUseProgram(0);
}

void Atmosphere::onResize(int newWidth, int newHeight)
{

}

void Atmosphere::onDelete()
{
	
}