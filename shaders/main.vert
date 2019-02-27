#version 400

// VBO-b�l �rkez� v�ltoz�k
layout(location = 0) in vec3 pos;

// a pipeline-ban tov�bb adand� �rt�kek
out gl_PerVertex
{
	vec4 gl_Position;
};

out VertexData
{
	vec3 out_pos;
};

uniform mat4 proj;

void main()
{
	out_pos = pos;
	gl_Position = proj * vec4(pos, 1);
}
