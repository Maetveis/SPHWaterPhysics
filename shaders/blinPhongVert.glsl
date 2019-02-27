#version 400

// VBO-ból érkezõ változók
layout(location = 0) in vec3 in_pos;
layout(location = 1) in vec3 in_norm;

// a pipeline-ban tovább adandó értékek
out VertexData
{
	vec3 out_norm;
	vec3 out_pos;
};

uniform mat4 model;

layout(std140) uniform FrameParams
{
	mat4 view;
	mat4 proj;
};

void main()
{
	vec4 pos = view * model * vec4(in_pos, 1);
	//No non-uniform scales because of this
	vec4 norm = view * model * vec4(in_norm, 0);
	gl_Position = proj * pos;
	
	out_pos = pos.xyz;
	out_norm = norm.xyz;
}