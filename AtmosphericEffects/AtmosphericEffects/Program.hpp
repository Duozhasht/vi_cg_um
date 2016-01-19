#ifndef PROGRAM_HPP
#define PROGRAM_HPP

#include "Shader.hpp"

/* TODO: Keep track of attached shaders? */

class Program
{
public:
	Program();

	void create();

	void attachShader(const Shader& s);
	bool compile();

	void use();

	void updateUniform(GLint loc, int value);
	void updateUniform(GLint loc, float value);

	GLint getUniformLocation(const char* uniform);

private:
	GLuint id;
};

#endif