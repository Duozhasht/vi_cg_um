#version 330

uniform float time;
uniform sampler2D tex;

in vec3 vColor;
in vec2 vTexCoord;

out vec4 color;

void main()
{
    color = vec4(vColor, 1);
}