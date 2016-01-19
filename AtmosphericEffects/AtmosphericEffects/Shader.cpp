#include "Shader.hpp"

#include <iostream>

#include <fstream>
#include <sstream>

Shader::Shader()
	:
	id(0)
{

}

bool Shader::loadFromFile(const std::string& path, Type type)
{
	std::string sCode;
	std::ifstream file;

	file.exceptions(std::ifstream::badbit);

	try
	{
		file.open(path);

		std::stringstream stream;
		stream << file.rdbuf();

		file.close();

		sCode = stream.str();
	}
	catch (std::ifstream::failure e)
	{
		std::cerr << "Error at Shader::loadFromFile: " << e.what() << std::endl;
		return false;
	}

	const GLchar* code = sCode.c_str();

	GLint success;
	GLchar infoLog[512];

	GLuint shaderType;

	switch (type)
	{
	case Vertex:
		shaderType = GL_VERTEX_SHADER;
		break;
	case Fragment:
		shaderType = GL_FRAGMENT_SHADER;
		break;
	case Geometry:
		shaderType = GL_GEOMETRY_SHADER;
		break;
	default:
		break;
	}

	id = glCreateShader(shaderType);

	glShaderSource(id, 1, &code, NULL);
	glCompileShader(id);

	glGetShaderiv(id, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(id, 512, NULL, infoLog);
		std::cerr << "Shader Compile Error: \"" << infoLog << "\"" << std::endl;
		return false;
	}

	return true;
}