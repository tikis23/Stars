#version 330 core

out vec4 fragColor;

// in
in vec3 v_color;
in vec3 fragPos;
in vec3 normal;

// uniform
uniform vec3 u_viewPos;
uniform vec4 u_color;
uniform bool u_isPlanet;
uniform bool u_isSun;
uniform float u_minHeight;
uniform float u_maxHeight;
uniform vec3 u_minColor;
uniform vec3 u_maxColor;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

vec3 getPhong();
void main()
{
    vec3 o_color = v_color * u_color.xyz;
    if(u_color.w > 0.9 && u_isPlanet == true)
    {
		float maxWeight = 0.1;
		float minWeight = 0.1;

		o_color = v_color * mix(u_color.xyz, u_maxColor, smoothstep(u_maxHeight - maxWeight, u_maxHeight, v_color.x));
		if(v_color.x < u_minHeight)
			o_color = mix(o_color, u_minColor, smoothstep(u_minHeight + minWeight, u_minHeight, v_color.x));
    }

	if(!u_isSun)
		o_color = o_color * getPhong();
    fragColor = vec4(o_color, u_color.w);
}

vec3 getPhong()
{
	vec3 lightPos = vec3(0, 0, 0);
	vec3 lightColor = vec3(1, 1, 1);
	float specStrength = 0.1;
	float shininess = 1;
	vec3 lightDir = normalize(lightPos - fragPos);
	vec3 viewDir = normalize(u_viewPos - fragPos);

	// ambient
	vec3 ambient = 0.025 * lightColor;
	
	// diffuse
	float diffuseFactor = 0.8 * max(dot(normal, lightDir), 0.0);
	vec3 diffuse = diffuseFactor * lightColor;

	// specular
	vec3 halfwayDir = normalize(lightDir + viewDir);  
	vec3 specular = specStrength * pow(max(dot(normal, halfwayDir), 0.0), shininess) * lightColor;
	if(diffuseFactor < 0)
		specular = vec3(0, 0, 0);
	return clamp(ambient + diffuse + specular, 0, 1.5);
}