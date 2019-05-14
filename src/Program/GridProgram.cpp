#include "GridProgram.hpp"

#include "../SPHSimulation/SimulationState.hpp"
#include <cmath>

#include <SDL2/SDL.h>

constexpr const char* indexBufferName = "indexBuffer";
constexpr const char* gridBufferName = "gridBuffer";
constexpr const char* positionBufferName = "positionBuffer";
constexpr const char* velocityBufferName = "velocityBuffer";
constexpr const char* positionNewBufferName = "positionNewBuffer";
constexpr const char* velocityNewBufferName = "velocityNewBuffer";
constexpr const char* superBlockBufferName = "superBlockBuffer";

GridProgram::GridProgram(SimulationState& _state) :
	state(_state)
{
	CompileShaders();

	state.AttachParticleIndex(count, indexBufferName);
	state.AttachGrid(count, gridBufferName);
	//state.AttachPosition(count, positionBufferName);

	state.AttachGrid(offset, gridBufferName);
	state.AttachSuperBlock(offset, superBlockBufferName);

	state.AttachSuperBlock(superBlock, superBlockBufferName);

	state.AttachGrid(finalize, gridBufferName);
	state.AttachSuperBlock(finalize, superBlockBufferName);

	state.AttachGrid(scatter, gridBufferName);
	state.AttachParticleIndex(scatter, indexBufferName);

}

bool CompileProgram(GL::Program& program, const char* source)
{
	GL::Shader shader(GL_COMPUTE_SHADER);
	if(!shader.FromFile(source))
	{
		Logger::Error() << "Shader compilation [" << source <<"] failed with message: " << shader.GetInfoLog() << '\n';
		return false;
	}

	program.AttachShader(shader);
	if(!program.Link())
	{
		Logger::Error() << "Shader linking failed with message: " << program.GetInfoLog() << '\n';
		return false;
	}

	return true;
}

void GridProgram::CompileShaders()
{
	CompileProgram(count, countSource);

	CompileProgram(offset, offsetSource);

	CompileProgram(superBlock, superBlockSource);

	CompileProgram(finalize, finalizeSource);

	CompileProgram(scatter, scatterSource);
}

void GridProgram::Run()
{
	glClearNamedBufferData(	state.GridBuffer().GetId(), GL_R32UI, GL_RED_INTEGER, GL_UNSIGNED_INT, nullptr);
	glMemoryBarrier(GL_BUFFER_UPDATE_BARRIER_BIT);

	count.Use();
	state.AttachPosition(count, positionBufferName);
	glUniform1ui(0, state.GridRes());
	glDispatchCompute(state.ResX() / 4, state.ResY() / 4, state.ResZ() / 4);

	glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

	//glFinish();

	offset.Use();
	//const unsigned l = state.GridRes() / 4;
	glDispatchCompute(1, 1, 1);

	glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

	//glFinish();

	superBlock.Use();
	//glUniform1ui(0, l * l * l * 64 / 1024);
	glDispatchCompute(1, 1, 1);
	glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

	//glFinish();

	finalize.Use();
	glDispatchCompute(state.GridRes() * state.GridRes() * state.GridRes() / 200, 1, 1);

	glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

	//glFinish();

	scatter.Use();
	state.AttachPosition(scatter, positionBufferName);
	state.AttachPositionBack(scatter, positionNewBufferName);
	state.AttachVelocity(scatter, velocityBufferName);
	state.AttachVelocityBack(scatter, velocityNewBufferName);
	glDispatchCompute(state.ResX() / 4, state.ResY() / 4, state.ResZ() / 4);
	glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

	//glFinish();

	state.SwapBuffers();
	glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT | GL_BUFFER_UPDATE_BARRIER_BIT);
}
