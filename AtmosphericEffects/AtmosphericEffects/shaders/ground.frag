#version 330

uniform vec4 ambient;
uniform vec4 diffuse;
uniform vec4 specular;
uniform float shininess;

in vec3 vNormal;
in vec3 vLightDir;
in vec3 vEye;

out vec4 color;

void main()
{
	// Since normalV was interpolated we must normalize again to ensure magnitude(normalV) == 1
	// No need to normalize ldV because light direction is constant
	vec3 n = normalize (vNormal);
	
	vec3 e = normalize(vEye);
	
	vec4 spec = vec4(0.0f);
	float intensity = max(0.0f, dot(vLightDir, n));
	
	if (intensity > 0)
	{
		h = normalize(e + vLightDir);
		float intensity = max(0, dot(h, n));
		spec = specular * pow(intensity * specular, shininess);
	}

	color = max(diffuse * 0.25, diffuse * intensity);
}