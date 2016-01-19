#ifndef MODEL_HPP
#define MODEL_HPP

#define GLEW_STATIC
#include <GL/glew.h>

#include <SFML/OpenGL.hpp>
#include "Vertex.hpp"

#include "Types.hpp"

#include "Program.hpp"

#include <vector>
using std::vector;

class Model
{
public:
	static Model plane(float width, float length);
	static Model cuboid(float width, float height, float length);
	static Model cone(float height, float radius, int slices, int stacks);
	static Model trunk(float height, float lRadius, float uRadius, int slices, int stacks);
	static Model sphere(float radius, int slices, int stacks);

public:
	Model();

	void create();
	void draw(const GLuint modelLoc);

	void rotate(vec3 axis, float angle);
	void translate(vec3 delta);

	void setTransform(const mat4& transform);

private:
	void addQuad(GLuint i1, GLuint i2, GLuint i3, GLuint i4);
	void addTri(GLuint i1, GLuint i2, GLuint i3);

	void addFace(vec3 v1, vec3 v2, vec3 v3, vec3 v4, vec3 normal);

	unsigned int addVertex(const vertex& v);

	vec3 position;
	mat4 transform;

	vector<vertex> vertices;
	vector<GLuint> indices;

	GLuint VAO, VBO, EBO;
};

#endif