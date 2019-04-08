#ifndef SIMULATION_STATE_HPP
#define SIMULATION_STATE_HPP

#include "../Helper/Buffer.hpp"
#include "../Helper/ShaderStorage.hpp"
#include "../Helper/Program.hpp"

#include <GL/glew.h>
#include <glm/vec3.hpp>

class SimulationState
{
private:
	GL::Buffer positionBuffer1;
	GL::Buffer positionBuffer2;
	GL::Buffer velocityBuffer1;
	GL::Buffer velocityBuffer2;
	GL::Buffer particleIndexBuffer;
	GL::Buffer gridBuffer;
	GL::Buffer superBlockBuffer;

	GL::Buffer pressureBuffer;
	GL::Buffer densityBufffer;
	GL::Buffer forceBuffer;

	GL::Buffer edgeBuffer;

	GL::ShaderStorage positionStorage1;
	GL::ShaderStorage positionStorage2;
	GL::ShaderStorage velocityStorage1;
	GL::ShaderStorage velocityStorage2;

	GL::ShaderStorage superBlockStorage;
	GL::ShaderStorage gridStorage;
	GL::ShaderStorage particleIndexStorage;

	GL::ShaderStorage pressureStorage;
	GL::ShaderStorage densityStorage;
	GL::ShaderStorage forceStorage;

	GL::ShaderStorage edgeStorage;

	const unsigned resX;
	const unsigned resY;
	const unsigned resZ;

	const GLuint gridResolution;

	bool firstIsForward;

	struct alignas(16) alignedVector;

	std::vector<alignedVector> MakeGrid();
	void InitBuffers();
public:
	SimulationState(unsigned _resX, unsigned _resY, unsigned _resZ, GLuint _gridResolution);

	void SwapBuffers();

	inline void AttachPosition(const GL::Program& program, const char* name)
	{
		if(firstIsForward)
		{
			positionStorage1.AttachToBlock(program, program.GetShaderStorageBlockIndex(name));
		}
		else
		{
			positionStorage2.AttachToBlock(program, program.GetShaderStorageBlockIndex(name));
		}
	}

	inline void AttachPositionBack(const GL::Program& program, const char* name)
	{
		if(firstIsForward)
		{
			positionStorage2.AttachToBlock(program, program.GetShaderStorageBlockIndex(name));
		}
		else
		{
			positionStorage1.AttachToBlock(program, program.GetShaderStorageBlockIndex(name));
		}
	}

	inline void AttachVelocity(const GL::Program& program, const char* name)
	{
		if(firstIsForward)
		{
			velocityStorage1.AttachToBlock(program, program.GetShaderStorageBlockIndex(name));
		}
		else
		{
			velocityStorage2.AttachToBlock(program, program.GetShaderStorageBlockIndex(name));
		}
	}

	inline void AttachVelocityBack(const GL::Program& program, const char* name)
	{
		if(firstIsForward)
		{
			velocityStorage2.AttachToBlock(program, program.GetShaderStorageBlockIndex(name));
		}
		else
		{
			velocityStorage1.AttachToBlock(program, program.GetShaderStorageBlockIndex(name));
		}
	}

	//TODO velocity same as above

	inline void AttachSuperBlock(const GL::Program& program, const char* name)
	{
		superBlockStorage.AttachToBlock(program, program.GetShaderStorageBlockIndex(name));
	}

	inline void AttachGrid(const GL::Program& program, const char* name)
	{
		gridStorage.AttachToBlock(program, program.GetShaderStorageBlockIndex(name));
	}

	inline void AttachParticleIndex(const GL::Program& program, const char* name)
	{
		particleIndexStorage.AttachToBlock(program, program.GetShaderStorageBlockIndex(name));
	}

	inline void AttachPressure(const GL::Program& program, const char* name)
	{
		pressureStorage.AttachToBlock(program, program.GetShaderStorageBlockIndex(name));
	}

	inline void AttachDensity(const GL::Program& program, const char* name)
	{
		densityStorage.AttachToBlock(program, program.GetShaderStorageBlockIndex(name));
	}

	inline void AttachForce(const GL::Program& program, const char* name)
	{
		forceStorage.AttachToBlock(program, program.GetShaderStorageBlockIndex(name));
	}

	inline void AttachEdge(const GL::Program& program, const char* name)
	{
		edgeStorage.AttachToBlock(program, program.GetShaderStorageBlockIndex(name));
	}

	inline unsigned GetEdgeCount()
	{
		unsigned* edgeCount = reinterpret_cast<unsigned*>(glMapNamedBufferRange(edgeBuffer.GetId(), 0, 4, GL_MAP_READ_BIT));
		glUnmapNamedBuffer(edgeBuffer.GetId());

		return *edgeCount;
	}

	inline void ResetEdgeCount()
	{
		unsigned* edgeCount = reinterpret_cast<unsigned*>(glMapNamedBufferRange(edgeBuffer.GetId(), 0, 4, GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_RANGE_BIT));
		*edgeCount = 0;
		glUnmapNamedBuffer(edgeBuffer.GetId());
	}

	inline unsigned ResX() const
	{
		return resX;
	}

	inline unsigned ResY() const
	{
		return resY;
	}

	inline unsigned ResZ() const
	{
		return resZ;
	}

	inline float GridRes() const
	{
		return gridResolution;
	}

	inline GL::Buffer& GridBuffer()
	{
		return gridBuffer;
	}

	inline GL::Buffer& EdgeBuffer()
	{
		return edgeBuffer;
	}
};

#endif //SIMULATION_STATE_HPP
