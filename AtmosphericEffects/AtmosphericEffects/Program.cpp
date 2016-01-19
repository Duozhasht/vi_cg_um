#include "Program.hpp"

#include <iostream>

Program::Program()
	:
	id(0)
{
	
}

void Program::create()
{
	id = glCreateProgram();
}

void Program::attachShader(const Shader& s)
{
	GLuint shaderId = s.getId();

	glAttachShader(id, shaderId);
}

bool Program::compile()
{
	glLinkProgram(id);

	GLint success;
	GLchar infoLog[512];

	glGetProgramiv(id, GL_LINK_STATUS, &success);

	if (!success)
	{
		glGetProgramInfoLog(id, 512, NULL, infoLog);
		std::cerr << "Shader Program Link Error: \"" << infoLog << "\"" << std::endl;

		return false;
	}

	return true;
}

void Program::use()
{
	glUseProgram(id);
}

GLint Program::getUniformLocation(const char* uniform)
{
	return glGetUniformLocation(id, uniform);
}
