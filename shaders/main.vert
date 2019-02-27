#version 400

// VBO-b�l �rkez� v�ltoz�k
layout(location = 0) in vec3 pos;
layout(location = 1) in vec3 color;

// a pipeline-ban tov�bb adand� �rt�kek
out VertexData
{
	vec4 out_col;
	vec4 out_pos;
};

uniform mat4 projection;
uniform mat4 modelView;

void main()
{
	out_pos = modelView * vec4( pos, 1 );
	gl_Position = projection * out_pos;
	out_col = vec4(color, 1);
}