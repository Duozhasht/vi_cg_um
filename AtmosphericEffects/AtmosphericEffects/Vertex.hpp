#ifndef VERTEX_HPP
#define VERTEX_HPP

#include "Types.hpp"

struct vertex
{
	vertex(const vec3& p = vec3(0.0f, 0.0f, 0.0f), const vec3& n = vec3(0.0f, 0.0f, 0.0f), const vec2 t = vec2(0.0f, 0.0f)) : position(p), normal(n), texCoord(t) {}

	vec3 position;
	vec3 normal;
	vec2 texCoord;
};

#endif