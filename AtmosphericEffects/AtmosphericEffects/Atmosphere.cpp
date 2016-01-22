#include "Atmosphere.hpp"
#include <iostream>

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/polar_coordinates.hpp>

const char* Atmosphere::AttributeNames[] = { "uPlanetRadius", "uAtmosRadius", "uRhlCoefs", "uMieCoef", "uSunInt", "uSunPos", "uRlhScaleH", "uMieScaleH", "g" };

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

	/*
	for (int i = PlanetRadius; i < TotalAttributes; ++i)
	{
		lAttributes[i] = shaders.getUniformLocation(AttributeNames[i]);
	}
	*/

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
	shaders.use();

	glUniformMatrix4fv(lView, 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(lProjection, 1, GL_FALSE, glm::value_ptr(projection));
	
	glUniform3f(lSunPosition, sunPosition.x, sunPosition.y, sunPosition.z);

	skyDome.draw(lModel);
	//sun.draw(lModel);

	// surface.draw(lModel);
}

void Atmosphere::onResize(int newWidth, int newHeight)
{

}

void Atmosphere::onDelete()
{
	
}