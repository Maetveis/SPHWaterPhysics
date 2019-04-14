#version 450

layout(std430, binding = 0) restrict readonly buffer positionBuffer
{
    vec3 position[];
};

/*struct gridIndex
{
    uint id;
    uint localOffset;
};

layout(std430) restrict readonly buffer indexBuffer
{
	gridIndex particleGridIndex[];
};*/

layout(std430) restrict readonly buffer pressureBuffer
{
    float pressure[];
};

layout(std430) restrict readonly buffer velocityBuffer
{
    vec3 velocity[];
};

layout(std430) restrict readonly buffer forceBuffer
{
    vec3 force[];
};

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
    /*uint id = gl_VertexID;

    vec3 pos = position[id];
    vec3 toTarget = -1.0 * pos;
    float l = length(toTarget);
    float strength = l * l - 1.0;

    position[id] = pos + strength * toTarget * 0.01;*/

    vec3 pos = position[gl_VertexID];
    //gridIndex grid = particleGridIndex[gl_VertexID];

    float pr = pressure[gl_VertexID];
	//out_pos = /*(pos + 1) / 2;//*/pr;//vec3(pr, 1 - pr, 0);//mix(vec3(1, 1, 1), vec3(0,0,0), vec3(grid.id / 32768.0));
    out_pos = vec3(0, .3, .7);//velocity[gl_VertexID];//force[gl_VertexID];//vec3(pr, 1 - pr, 0);

	gl_Position = proj * vec4(pos, 1);
    //gl_PointSize = 5 - 2 * gl_Position[3];
}
