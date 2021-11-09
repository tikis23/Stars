#version 330 core

layout(triangles) in;
layout(triangle_strip, max_vertices = 6) out;

in VertexData
{
    vec3 color;
} vertex[];


uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

out vec3 v_color;
out vec3 fragPos;
out vec3 normal;

void main()
{ 	

	mat4 MVP = projection * view * model;
	mat3 normalMatrix = mat3(transpose(inverse(model)));

	v_color = vertex[0].color;
	fragPos = (model * gl_in[0].gl_Position).xyz;
	normal = vec3(vec4(normalMatrix * normalize(cross(gl_in[1].gl_Position.xyz - gl_in[0].gl_Position.xyz, gl_in[2].gl_Position.xyz - gl_in[0].gl_Position.xyz)), 0.0));
	gl_Position =  MVP * gl_in[0].gl_Position;
	EmitVertex();

	v_color = vertex[1].color;
	fragPos = (model * gl_in[1].gl_Position).xyz;
	normal = vec3(vec4(normalMatrix * normalize(cross(gl_in[2].gl_Position.xyz - gl_in[1].gl_Position.xyz, gl_in[0].gl_Position.xyz - gl_in[1].gl_Position.xyz)), 0.0));
	gl_Position =  MVP * gl_in[1].gl_Position;
	EmitVertex();


	v_color = vertex[2].color;
	fragPos = (model * gl_in[2].gl_Position).xyz;
	normal = vec3(vec4(normalMatrix * normalize(cross(gl_in[0].gl_Position.xyz - gl_in[2].gl_Position.xyz, gl_in[1].gl_Position.xyz - gl_in[2].gl_Position.xyz)), 0.0));
	gl_Position =  MVP * gl_in[2].gl_Position;
	EmitVertex();
		
	EndPrimitive();
    
}