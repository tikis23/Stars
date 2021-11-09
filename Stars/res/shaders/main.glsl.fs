#version 330 core

out vec4 fragColor;

// in
in vec3 v_color;
in vec3 fragPos;
in vec3 normal;

// uniform
uniform vec3 u_viewPos;
uniform vec4 u_color;
//uniform vec2 u_Resolution;
uniform bool u_isPlanet;
uniform bool u_isSun;
uniform bool u_hasRing;
uniform float u_minHeight;
uniform float u_maxHeight;


uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

vec3 getPhong();
void main()
{
    vec3 o_color = v_color * u_color.xyz;
    if(u_color.w > 0.9 && u_isPlanet == true)
    {
        if(v_color.x < u_minHeight)
        {  
            o_color = u_color.xyz * vec3(u_color.z + v_color.x, u_color.x + v_color.x, u_color.y + v_color.x) * 0.5;
        }
        else if(v_color.x > u_maxHeight)
        {  
            o_color = u_color.xyz * (2+v_color.x);
        }
        else
            o_color = v_color * u_color.xyz * 0.8;
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
	float diffuseFactor = 0.5 * max(dot(normal, lightDir), 0.0);
	vec3 diffuse = diffuseFactor * lightColor;

	// specular
	vec3 halfwayDir = normalize(lightDir + viewDir);  
	vec3 specular = specStrength * pow(max(dot(normal, halfwayDir), 0.0), shininess) * lightColor;

	return clamp(ambient + diffuse + specular, 0, 1.5);
}