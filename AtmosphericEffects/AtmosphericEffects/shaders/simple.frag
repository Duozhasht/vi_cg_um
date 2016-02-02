#version 330

uniform sampler2D tex;

in vec3 vNormal;
in vec2 vTexCoord;

in vec3 vLightDir;

out vec4 color;

void main()
{
	vec3 n = normalize(vNormal);
	
	float intensity = max(0.3f, dot(vLightDir, n));

    color = intensity * texture(tex, 100 * vTexCoord);
}