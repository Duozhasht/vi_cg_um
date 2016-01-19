#version 330

uniform float time;
uniform sampler2D tex;

in vec3 colorF;
in vec2 texCoordF;

out vec4 color;

void main()
{
    color = vec4(colorF, 1);
}