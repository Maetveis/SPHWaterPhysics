#version 450

const vec4 quadPoints[4] =
{
	vec4(-1.0,  1.0, 0.0, 1.0),
	vec4(-1.0, -1.0, 0.0, 1.0),
	vec4( 1.0,  1.0, 0.0, 1.0),
	vec4( 1.0, -1.0, 0.0, 1.0)
};

out gl_PerVertex
{
	vec4 gl_Position;
};

out FragmentData
{
	vec3 rayStart;
};

layout(location = 0) uniform mat4 world;

void main()
{
	gl_Position = quadPoints[gl_VertexID];

	rayStart = (world * quadPoints[gl_VertexID]).xyz;
}
