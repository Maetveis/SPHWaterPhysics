#version 450

layout(std430, binding = 0) restrict readonly buffer positionBuffer
{
    vec3 position[];
};

struct gridIndex
{
    uint id;
    uint localOffset;
};

layout(std430) restrict readonly buffer indexBuffer
{
	gridIndex particleGridIndex[];
};

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
    /*uint id = gl_VertexID;

    vec3 pos = position[id];
    vec3 toTarget = -1.0 * pos;
    float l = length(toTarget);
    float strength = l * l - 1.0;

    position[id] = pos + strength * toTarget * 0.01;*/

    vec3 pos = position[gl_VertexID];
    gridIndex grid = particleGridIndex[gl_VertexID];

	out_pos = mix(vec3(1, 1, 1), vec3(0,0,0), vec3(grid.id / 32768.0, (grid.id % 1024) / 1024.0, (grid.id % 32) / 32.0));
	gl_Position = proj * vec4(pos, 1);
}
