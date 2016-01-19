#include "Model.hpp"

#include <iostream>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/constants.hpp>

#include <glm/gtx/string_cast.hpp>

using namespace std;

Model Model::plane(float width, float length)
{
	Model m;

	vertex v1, v2, v3, v4;

	v1.position = { -width / 2.f, 0.0f, -length / 2.f };
	v2.position = { width / 2.f, 0.0f, -length / 2.f };
	v3.position = { -width / 2.f, 0.0f, length / 2.f };
	v4.position = { width / 2.f, 0.0f, length / 2.f };

	v1.normal = { 0.0f, 1.0f, 0.0f };
	v2.normal = { 0.0f, 1.0f, 0.0f };
	v3.normal = { 0.0f, 1.0f, 0.0f };
	v4.normal = { 0.0f, 1.0f, 0.0f };

	v1.texCoord = { 0.0f, 0.0f };
	v2.texCoord = { 1.0f, 0.0f };
	v3.texCoord = { 1.0f, 1.0f };
	v4.texCoord = { 0.0f, 1.0f };

	GLuint i1 = m.addVertex(v1);
	GLuint i2 = m.addVertex(v2);
	GLuint i3 = m.addVertex(v3);
	GLuint i4 = m.addVertex(v4);

	m.addQuad(i3, i4, i1, i2);

	m.create();

	return m;
}

Model Model::cuboid(float width, float height, float length)
{
	Model m;

	vec3 v1 = { -width / 2.f, height / 2, -length / 2.f };
	vec3 v2 = { width / 2.f, height / 2, -length / 2.f };
	vec3 v3 = { -width / 2.f, height / 2, length / 2.f };
	vec3 v4 = { width / 2.f, height / 2, length / 2.f };

	vec3 v5 = { -width / 2.f, -height / 2, -length / 2.f };
	vec3 v6 = { width / 2.f, -height / 2, -length / 2.f };
	vec3 v7 = { -width / 2.f, -height / 2, length / 2.f };
	vec3 v8 = { width / 2.f, -height / 2, length / 2.f };

	vec3 n1 = { 0.0f, 1.0f, 0.0f };
	vec3 n2 = { 0.0f, -1.0f, 0.0f };

	vec3 n3 = { 1.0f, 0.0f, 0.0f };
	vec3 n4 = { -1.0f, 0.0f, 0.0f };

	vec3 n5 = { 0.0f, 0.0f, 1.0f };
	vec3 n6 = { 0.0f, 0.0f, -1.0f };

	m.addFace(v1, v2, v3, v4, n1); // top
	m.addFace(v6, v5, v8, v7, n2); // bottom
	m.addFace(v4, v2, v8, v6, n3); // right
	m.addFace(v1, v3, v5, v7, n4); // left
	m.addFace(v3, v4, v7, v8, n5); // front
	m.addFace(v2, v1, v6, v5, n6); // back

	m.create();

	return m;
}

Model Model::cone(float height, float radius, int slices, int stacks)
{
	Model m;

	float angleInc = 2 * glm::pi<float>() / (float)slices;
	float angle = 0;

	vec2 slope;

	slope.x = radius / (float)stacks;
	slope.y = height / (float)stacks;

	vec2 texInc;
	texInc.s = 1.0f / (float)slices;
	texInc.t = 1.0f / (float)stacks;

	vec2 texCoords;

	vector<vector<vertex>> mesh(slices+1);
	vertex v;
	vec3 n;

	// normalizing
	float h = sqrt(height*height + radius*radius);

	n.y = radius / h;
	
	for (int i = 0; i <= slices; ++i)
	{
		n.x = height * cos(angle) / h;
		n.z = height * sin(angle) / h;

		texCoords.t = 1;

		for (int j = 0; j < stacks; ++j)
		{
			v.position.x = (radius - j*slope.x) * cos(angle);
			v.position.y = j * slope.y;
			v.position.z = (radius - j*slope.x) * sin(angle);

			v.normal = n;
			v.texCoord = texCoords;

			mesh[i].push_back(v);

			texCoords.t -= texInc.t;
		}

		angle += angleInc;
		texCoords.s += texInc.s;
	}

	vertex top, bot;
	GLuint topI, botI;

	top.position = vec3(0.f, height, 0.f);

	bot.position = vec3(0.f, 0.0f, 0.0f);
	bot.normal = vec3(0.f, -1.f, 0.0f);

	botI = m.addVertex(bot);
	
	vertex v1, v2, v3, v4;
	GLuint i1, i2, i3, i4;

	vector<GLuint> seam[2];

	for (int j = 0; j < stacks; ++j)
	{
		seam[0].push_back(m.addVertex(mesh[0][j]));
	}

	for (int i = 0; i < slices; ++i)
	{
		v2 = mesh[i+1][0];

		i1 = seam[0][0];
		i2 = m.addVertex(v2);

		seam[1].push_back(i2);

		m.addTri(i1, botI, i2);

		for (int j = 1; j < stacks; ++j)
		{
			v4 = mesh[i + 1][j];

			i3 = seam[0][j];
			i4 = m.addVertex(v4);

			seam[1].push_back(i4);
			
			m.addQuad(i3, i4, i1, i2);

			i1 = i3;
			i2 = i4;
		}

		top.normal = v2.normal;
		top.texCoord = vec2(v2.texCoord.x, 0.f);

		topI = m.addVertex(top);

		m.addTri(i4, topI, i3);
		seam[0] = seam[1];
		seam[1].clear();
	}

	m.create();

	return m;
}

Model Model::trunk(float height, float lRadius, float uRadius, int slices, int stacks)
{
	Model m;

	float angleInc = 2 * glm::pi<float>() / (float)slices;
	float angle = 0;

	float r = lRadius - uRadius;

	vec2 slope;

	slope.x = r / (float)stacks;
	slope.y = height / (float)(stacks-1);

	vec2 texInc;
	texInc.s = 1.0f / (float)slices;
	texInc.t = 1.0f / (float)stacks;

	vec2 texCoords;

	vector<vector<vertex>> mesh(slices + 1);
	vertex v;
	vec3 n;

	// normalizing
	float h = sqrt(height*height + r*r);

	n.y = r / h;

	for (int i = 0; i <= slices; ++i)
	{
		n.x = height * cos(angle) / h;
		n.z = height * sin(angle) / h;

		texCoords.t = 1;

		for (int j = 0; j < stacks; ++j)
		{
			v.position.x = (lRadius - j*slope.x) * cos(angle);
			v.position.y = j * slope.y;
			v.position.z = (lRadius - j*slope.x) * sin(angle);

			v.normal = n;
			v.texCoord = texCoords;

			mesh[i].push_back(v);

			texCoords.t -= texInc.t;
		}

		angle += angleInc;
		texCoords.s += texInc.s;
	}

	vertex top, bot;
	GLuint topI, botI;

	top.position = vec3(0.f, height, 0.f);

	bot.position = vec3(0.f, 0.0f, 0.0f);
	bot.normal = vec3(0.f, -1.f, 0.0f);

	botI = m.addVertex(bot);

	vertex v1, v2, v3, v4;
	GLuint i1, i2, i3, i4;

	vector<GLuint> seam[2];

	for (int j = 0; j < stacks; ++j)
	{
		seam[0].push_back(m.addVertex(mesh[0][j]));
	}

	for (int i = 0; i < slices; ++i)
	{
		v2 = mesh[i + 1][0];

		i1 = seam[0][0];
		i2 = m.addVertex(v2);

		seam[1].push_back(i2);

		m.addTri(i1, botI, i2);

		for (int j = 1; j < stacks; ++j)
		{
			v4 = mesh[i + 1][j];

			i3 = seam[0][j];
			i4 = m.addVertex(v4);

			seam[1].push_back(i4);

			m.addQuad(i3, i4, i1, i2);

			i1 = i3;
			i2 = i4;
		}

		top.normal = v2.normal;
		top.texCoord = vec2(v2.texCoord.x, 0.f);

		topI = m.addVertex(top);

		m.addTri(i4, topI, i3);
		seam[0] = seam[1];
		seam[1].clear();
	}

	m.create();

	return m;
}

Model Model::sphere(float radius, int slices, int stacks)
{
	float xAngle = 0.0f;
	float yAngle = 0.0f;

	float pi = glm::pi<float>();
	float half_pi = glm::half_pi<float>();

	float xAngleInc = 2 * pi / (float)slices;
	float yAngleInc = pi / (float)stacks;

	vec2 texInc;
	texInc.s = 1.0f / (float)slices;
	texInc.t = 1.0f / (float)stacks;

	vec2 texCoords(0.0f, 0.0f);

	vector<vector<vertex>> mesh(slices + 1);
	vertex v;
	vec3 n;

	for (int i = 0; i <= slices; ++i)
	{
		texCoords.t = texInc.t;
		yAngle = half_pi - yAngleInc;

		for (int j = 0; j < stacks; ++j)
		{
			v.normal.x = sin(xAngle) * cos(yAngle);
			v.normal.y = sin(yAngle);
			v.normal.z = cos(xAngle) * cos(yAngle);

			normalize(v.normal);

			v.position.x = radius * v.normal.x;
			v.position.y = radius * v.normal.y;
			v.position.z = radius * v.normal.z;

			v.texCoord = texCoords;

			mesh[i].push_back(v);

			texCoords.t -= texInc.t;
			yAngle -= yAngleInc;
		}

		texCoords.x += texInc.s;
		xAngle += xAngleInc;
	}

	Model m;

	vertex top, bot;
	GLuint topI, botI;

	top.position = vec3(0.f, radius, 0.f);
	top.normal = vec3(0.0f, 1.0f, 0.0f);

	bot.position = vec3(0.f, -radius, 0.0f);
	bot.normal = vec3(0.f, -1.f, 0.0f);

	botI = m.addVertex(bot);

	vertex v1, v2, v3, v4;
	GLuint i1, i2, i3, i4;

	vector<GLuint> seam[2];

	for (int j = 0; j < stacks; ++j)
	{
		seam[0].push_back(m.addVertex(mesh[0][j]));
	}

	for (int i = 0; i < slices; ++i)
	{
		v2 = mesh[i + 1][0];

		i1 = seam[0][0];
		i2 = m.addVertex(v2);

		seam[1].push_back(i2);

		top.texCoord = vec2(v2.texCoord.x, 0.0f);

		topI = m.addVertex(top);

		m.addTri(i1, topI, i2);

		for (int j = 1; j < stacks; ++j)
		{
			v4 = mesh[i + 1][j];

			i3 = seam[0][j];
			i4 = m.addVertex(v4);

			seam[1].push_back(i4);

			m.addQuad(i3, i4, i1, i2);

			i1 = i3;
			i2 = i4;
		}

		bot.texCoord = vec2(v2.texCoord.x, 1.0f);

		botI = m.addVertex(bot);

		m.addTri(i4, botI, i3);

		seam[0] = seam[1];
		seam[1].clear();
	}

	m.create();
	
	return m;
}
 
Model::Model()
{

}

void Model::create()
{
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vertex), vertices.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);

	// Vertex Positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (GLvoid*)0);
	// Vertex Normals
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (GLvoid*)offsetof(vertex, normal));
	// Vertex Texture Coords
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(vertex), (GLvoid*)offsetof(vertex, texCoord));

	glBindVertexArray(0);
}

void Model::draw(const GLuint modelLoc)
{
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(transform));

	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void Model::rotate(vec3 axis, float angle)
{
	transform = glm::rotate(transform, angle, axis);
}

void Model::translate(vec3 delta)
{
	transform = glm::translate(transform, delta);
}

void Model::setTransform(const mat4& t)
{
	transform = t;
}

void Model::addQuad(GLuint i1, GLuint i2, GLuint i3, GLuint i4)
{
	addTri(i1, i3, i2);
	addTri(i3, i4, i2);
}

void Model::addTri(GLuint i1, GLuint i2, GLuint i3)
{
	indices.push_back(i3);
	indices.push_back(i2);
	indices.push_back(i1);
}

unsigned int Model::addVertex(const vertex& v)
{
	vertices.push_back(v);

	return vertices.size() - 1;
}

void Model::addFace(vec3 v1, vec3 v2, vec3 v3, vec3 v4, vec3 normal)
{
	GLuint i1 = addVertex(vertex(v1, normal));
	GLuint i2 = addVertex(vertex(v2, normal));
	GLuint i3 = addVertex(vertex(v3, normal));
	GLuint i4 = addVertex(vertex(v4, normal));

	addQuad(i3, i4, i1, i2);
}