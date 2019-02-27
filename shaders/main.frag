#version 400

in VertexData
{
	vec4 out_col;
	vec4 out_pos;
};

layout(location = 0) out vec4 fs_out_col;

void main()
{
	fs_out_col = out_col;
}