#version 330

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoord;

uniform vec4 uLightDirection; // In global space

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 vNormal;
out vec3 vLightDir;
out vec3 vEye;

void main()
{
	vLightDir = normalize (view * model * uLightDirection);
	
	// Make sure that the direction is correct
	vNormal = normalize (view * model * normal);
	
	vEye = -vec3(view * model * position);
	
	gl_Position = projection * view * model * position;
}