#version 400

in VertexData
{
	float out_pos;
};

layout(location = 0) out vec4 fs_out_col;

void main()
{
	//if(out_pos < 0.14)
	//	discard;

	fs_out_col = vec4(out_pos, 1 - out_pos, 0, 1);
}
