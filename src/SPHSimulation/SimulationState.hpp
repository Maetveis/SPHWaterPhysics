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

	GL::ShaderStorage positionStorage1;
	GL::ShaderStorage positionStorage2;
	GL::ShaderStorage velocityStorage1;
	GL::ShaderStorage velocityStorage2;

	GL::ShaderStorage superBlockStorage;
	GL::ShaderStorage gridStorage;
	GL::ShaderStorage particleIndexStorage;

	GL::ShaderStorage pressureStorage;

	/*static constexpr GLuint Position = 0;
	static constexpr GLuint PositionTemp = 1;
	static constexpr GLuint Velocity = 2;
	static constexpr GLuint VelocityTemp = 3;
	static constexpr GLuint Index = 4;
	static constexpr GLuint Grid = 5;
	static constexpr GLuint SuperBlock = 6;*/

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
};

#endif //SIMULATION_STATE_HPP
