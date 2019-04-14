#version 450

layout(std430) restrict buffer edgeBuffer
{
    uint count;
    vec3 position[];
} edgeParticles;

out gl_PerVertex
{
	vec4 gl_Position;
    float gl_PointSize;
};

out VertexData
{
	vec3 out_pos;
};

layout(location = 0) uniform mat4 proj;

void main()
{
	vec3 pos = gl_VertexID < edgeParticles.count ? edgeParticles.position[gl_VertexID] : vec3(-2, -2, -2);
    out_pos = vec3(0, .4, 1);

	gl_Position = proj * vec4(pos, 1);
}
