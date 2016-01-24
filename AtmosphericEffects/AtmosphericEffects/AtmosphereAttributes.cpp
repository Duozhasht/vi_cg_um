#include "AtmosphereAttributes.hpp"

#include <fstream>
#include <sstream>
#include <iostream>

#include <glm\gtc\constants.hpp>

#include "rapidjson/document.h"
#include "rapidjson\error\error.h"
#include "rapidjson\error\en.h"

AtmosphereAttributes::AtmosphereAttributes(float pr, float ar, vec3 rlh, float mie, float sunI, float rlhSh, float mieSh, float g)
	:
	PlanetRadius(pr),
	AtmosphereRadius(ar),
	RayleighCoefs(rlh),
	MieCoef(mie),
	SunIntensity(sunI),
	RayleighScaleH(rlhSh),
	MieScaleH(mieSh),
	G(g)
{

}

bool AtmosphereAttributes::load(const char *json)
{
	std::ifstream file;
	std::string contents;

	file.exceptions(std::ifstream::badbit);

	try
	{
		file.open(json);

		std::stringstream stream;
		stream << file.rdbuf();

		file.close();

		contents = stream.str();
	}
	catch (std::ifstream::failure e)
	{
		std::cerr << "Error at AtmosphereAttributes::load: " << e.what() << std::endl;
		return false;
	}

	rapidjson::Document doc;
	rapidjson::ParseResult ok = doc.Parse(contents.c_str());

	if (!ok)
	{
		std::cerr << "JSON parse error at AtmosphereAttributes::load: " << rapidjson::GetParseError_En(ok.Code()) << std::endl;
	}

	PlanetRadius	=	doc["PlanetRadius"].GetDouble();
	AtmosphereRadius =	doc["AtmosphereRadius"].GetDouble();

	RayleighCoefs.r = doc["RayleighR"].GetDouble();
	RayleighCoefs.g = doc["RayleighG"].GetDouble();
	RayleighCoefs.b = doc["RayleighB"].GetDouble();

	MieCoef =			doc["MieCoef"].GetDouble();
	SunIntensity =		doc["SunIntensity"].GetDouble();
	RayleighScaleH =	doc["RayleighScaleH"].GetDouble();
	MieScaleH =			doc["MieScaleH"].GetDouble();
	G = 				doc["G"].GetDouble();
}

void AtmosphereAttributes::calculateRayleigh()
{
	float wlR = 0.650f;		// 650 nm for red
	float wlG = 0.570f;		// 570 nm for green
	float wlB = 0.475f;		// 475 nm for blue

	float pi = glm::pi<float>();

	float gammaR = pow(wlR, 4);
	float gammaG = pow(wlG, 4);
	float gammaB = pow(wlB, 4);

	float q = 8 * pow(pi, 3) * pow(pow(n, 2) - 1, 2) / (3 * N);

	RayleighCoefs.r = q * 1 / gammaR;
	RayleighCoefs.g = q * 1 / gammaG;
	RayleighCoefs.b = q * 1 / gammaB;
}