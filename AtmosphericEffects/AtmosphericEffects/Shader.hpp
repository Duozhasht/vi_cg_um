#ifndef SHADER_HPP
#define SHADER_HPP

#include <string>

#define GLEW_STATIC
#include <GL/glew.h>

#include <SFML/OpenGL.hpp>

class Shader
{
public:
	enum Type
	{
		Vertex,
		Fragment,
		Geometry
	};
public:
	Shader();

	bool loadFromFile(const std::string& path, Type type);

	GLuint getId() const { return id; }

private:
	GLuint id;

};

#endif