#version 330 core
  
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform vec3 uSunPos;

out vec3 vNormal;
out vec2 vTexCoord;

out vec3 vLightDir;

void main()
{
    gl_Position = projection * view * model * vec4(position, 1.0);

	vNormal = normal;
	vTexCoord = texCoord;

	vLightDir = normalize (uSunPos);
}