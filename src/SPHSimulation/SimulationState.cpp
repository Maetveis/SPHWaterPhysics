#include "SimulationState.hpp"

#include <glm/vec3.hpp>
#include <cmath>

SimulationState::SimulationState(unsigned _resX, unsigned _resY, unsigned _resZ, GLuint _gridResolution) :
	resX(_resX),
	resY(_resY),
	resZ(_resZ),
	gridResolution(_gridResolution),
	firstIsForward(true)
{
	InitBuffers();

	positionStorage1.AttachBuffer(positionBuffer1);
	positionStorage2.AttachBuffer(positionBuffer2);

	velocityStorage1.AttachBuffer(velocityBuffer1);
	velocityStorage2.AttachBuffer(velocityBuffer2);

	superBlockStorage.AttachBuffer(superBlockBuffer);
	gridStorage.AttachBuffer(gridBuffer);
	particleIndexStorage.AttachBuffer(particleIndexBuffer);

	pressureStorage.AttachBuffer(pressureBuffer);
	densityStorage.AttachBuffer(densityBufffer);

	forceStorage.AttachBuffer(forceBuffer);
}

struct alignas(16) SimulationState::alignedVector
{
	glm::vec3 pos;
	alignedVector(const glm::vec3 _pos) :
		pos(_pos)
	{
	}

	alignedVector(float c1, float c2, float c3) :
		pos(c1, c2, c3)
	{
	}
};


std::vector<SimulationState::alignedVector> SimulationState::MakeGrid()
{
	std::vector<alignedVector> data;

	data.reserve(resX * resY * resZ);

	const float multX = 1. / resX;
	const float multY = 2. / resY;
	const float multZ = 2. / resZ;

	for(size_t x = 0; x < resX; ++x)
	{
		for(size_t y = 0; y < resY; ++y)
		{
			for(size_t z = 0; z < resZ; ++z)
			{
				data.emplace_back(x * multX/* - 1.0*/, y * multY - 1.0, z * multZ - 1.0);
			}
		}
	}

	return data;
}

void SimulationState::InitBuffers()
{
	auto data = MakeGrid();
	positionBuffer1.BufferData(data, GL_DYNAMIC_COPY);
	positionBuffer2.InitEmpty(data.size() * sizeof(data[0]), GL_DYNAMIC_COPY);

	velocityBuffer1.InitEmpty(data.size() * sizeof(data[0]), GL_DYNAMIC_COPY);
	glClearNamedBufferData(	velocityBuffer1.GetId(), GL_RGBA32F, GL_RED, GL_FLOAT, nullptr);

	velocityBuffer2.InitEmpty(data.size() * sizeof(data[0]), GL_DYNAMIC_COPY);

	forceBuffer.InitEmpty(data.size() * sizeof(data[0]), GL_DYNAMIC_COPY);

	particleIndexBuffer.InitEmpty(2 * data.size() * sizeof(GLuint), GL_DYNAMIC_COPY);
	unsigned edgeLength = gridResolution;
	gridBuffer.InitEmpty(edgeLength * edgeLength * edgeLength * sizeof(GLuint), GL_DYNAMIC_COPY);
	superBlockBuffer.InitEmpty(edgeLength * edgeLength * edgeLength * sizeof(GLuint) / 512, GL_DYNAMIC_COPY);

	pressureBuffer.InitEmpty(data.size() * sizeof(GLfloat), GL_DYNAMIC_COPY);
	densityBufffer.InitEmpty(data.size() * sizeof(GLfloat), GL_DYNAMIC_COPY);

	//Note to self: forgeting syncronization screws things up so dont do it
	glMemoryBarrier(GL_BUFFER_UPDATE_BARRIER_BIT);
}

void SimulationState::SwapBuffers()
{
	firstIsForward = !firstIsForward;
}
