#include "Atmosphere.hpp"
#include <iostream>

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/polar_coordinates.hpp>

#include <SFML/Graphics.hpp>

const char* Atmosphere::AttributeNames[] = { "uPlanetRadius", "uAtmosHeight", "uRhlCoefs", "uMieCoef", "uSunInt", "uRlhScaleH", "uMieScaleH", "uG" };

Atmosphere::Atmosphere()
{
	
}

bool Atmosphere::loadAttributes(const char *file)
{
	if (!attributes.load("config/atts.json"))
		return false;

	setUniformAttributes();

	return true;
}

void Atmosphere::setUniformAttributes()
{
	glUseProgram(skyShaders.id);

	glUniform1f(lAttributes[PlanetRadius], attributes.PlanetRadius);
	glUniform1f(lAttributes[AtmosphereHeight], attributes.AtmosphereHeight);
	glUniform3f(lAttributes[RayleighCoefs], attributes.RayleighCoefs.r, attributes.RayleighCoefs.g, attributes.RayleighCoefs.b);

	glUniform1f(lAttributes[MieCoef], attributes.MieCoef);
	glUniform1f(lAttributes[SunIntensity], attributes.SunIntensity);

	glUniform1f(lAttributes[RayleighScaleH], attributes.RayleighScaleH);
	glUniform1f(lAttributes[MieScaleH], attributes.MieScaleH);
	glUniform1f(lAttributes[G], attributes.G);

	glUseProgram(0);
}


bool Atmosphere::create(float r)
{
	if(!skyShaders.load("shaders/atmosphere.vert", "shaders/atmosphere.frag"))
		return false;

	for (int i = PlanetRadius; i < TotalAttributes; ++i)
	{
		lAttributes[i] = skyShaders.getUniformLocation(AttributeNames[i]);
	}

	if(!loadAttributes("config/atts.json"))
		return false;

	lSunPositionS = skyShaders.getUniformLocation("uSunPos");

	lModelS = skyShaders.getUniformLocation("model");
	lViewS = skyShaders.getUniformLocation("view");
	lProjectionS = skyShaders.getUniformLocation("projection");

	if (!groundShaders.load("shaders/simple.vert", "shaders/simple.frag"))
		return false;

	lSunPositionG = groundShaders.getUniformLocation("uSunPos");
	lModelG = groundShaders.getUniformLocation("model");
	lViewG = groundShaders.getUniformLocation("view");
	lProjectionG = groundShaders.getUniformLocation("projection");

	sf::Image image;

	if (!image.loadFromFile("sand.jpg"))
	{
		std::cerr << "Error loading texture" << std::endl;
	}

	GLfloat largest;
	glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &largest);

	glGenTextures(1, &surfaceTex);

	glBindTexture(GL_TEXTURE_2D, surfaceTex);
	
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, largest);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image.getSize().x, image.getSize().y, 0, GL_RGBA, GL_UNSIGNED_BYTE, image.getPixelsPtr());
	glGenerateMipmap(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, 0);

	if (!image.loadFromFile("stars.jpg"))
	{
		std::cerr << "Error loading texture" << std::endl;
	}

	glGenTextures(1, &starsTex);

	glBindTexture(GL_TEXTURE_2D, starsTex);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image.getSize().x, image.getSize().y, 0, GL_RGBA, GL_UNSIGNED_BYTE, image.getPixelsPtr());
	glGenerateMipmap(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, 0);

	lModelG = groundShaders.getUniformLocation("model");
	lViewG = groundShaders.getUniformLocation("view");
	lProjectionG = groundShaders.getUniformLocation("projection");

	sunPolar = vec2(-0.4f, 0.0f);
	radius = r;

	skyDome = Model::dome(radius, 100, 100);
	surface = Model::plane(2 * radius, 2 * radius);

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
	glUseProgram(skyShaders.id);
	glBindTexture(GL_TEXTURE_2D, starsTex);

	glUniformMatrix4fv(lViewS, 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(lProjectionS, 1, GL_FALSE, glm::value_ptr(projection));
	
	glUniform3f(lSunPositionS, sunPosition.x, sunPosition.y, sunPosition.z);

	skyDome.draw(lModelS);

	glBindTexture(GL_TEXTURE_2D, 0);
	glUseProgram(0);

	glUseProgram(groundShaders.id);
	glUniform3f(lSunPositionG, sunPosition.x, sunPosition.y, sunPosition.z);

	glUniformMatrix4fv(lViewG, 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(lProjectionG, 1, GL_FALSE, glm::value_ptr(projection));

	glBindTexture(GL_TEXTURE_2D, surfaceTex);

	surface.draw(lModelG);

	glBindTexture(GL_TEXTURE_2D, 0);

	glUseProgram(0);

	//sun.draw(lModel);

	// surface.draw(lModel);

	
}