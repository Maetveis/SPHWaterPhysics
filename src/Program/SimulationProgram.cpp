#include "SimulationProgram.hpp"

#include "../SPHSimulation/SimulationState.hpp"

#include <SDL2/SDL.h>

namespace
{

constexpr const char* positionBufferName = "positionBuffer";
constexpr const char* densityBufferName = "densityBuffer";
constexpr const char* pressureBufferName = "pressureBuffer";
constexpr const char* forceBufferName = "forceBuffer";
constexpr const char* velocityBufferName = "velocityBuffer";
constexpr const char* gridBufferName = "gridBuffer";

bool CompileProgram(GL::Program& program, const char* source)
{
	GL::Shader shader(GL_COMPUTE_SHADER);
	if(!shader.FromFile(source))
	{
		Logger::Error << "Shader compilation [" << source <<"] failed with message: " << shader.GetInfoLog() << '\n';
		return false;
	}

	program.AttachShader(shader);
	if(!program.Link())
	{
		Logger::Error << "Shader linking failed with message: " << program.GetInfoLog() << '\n';
		return false;
	}

	return true;
}

} //unnamed namespace

SimulationProgram::SimulationProgram(SimulationState& _state) :
	state(_state)
{
	CompileShaders();

	state.AttachPressure(pressure, pressureBufferName);
	state.AttachDensity(pressure, densityBufferName);
	state.AttachGrid(pressure, gridBufferName);

	state.AttachPressure(force, pressureBufferName);
	state.AttachDensity(force, densityBufferName);
	state.AttachGrid(force, gridBufferName);
	state.AttachForce(force, forceBufferName);
}

void SimulationProgram::CompileShaders()
{
	CompileProgram(pressure, pressureSource);
	CompileProgram(force, forceSource);
}

void SimulationProgram::Run()
{
	pressure.Use();
	state.AttachPosition(pressure, positionBufferName);

	glUniform1f(0, 0.15);
	glUniform1f(1, 5);
	glUniform1f(2, 2000);

	glDispatchCompute(state.GridRes(), state.GridRes(), state.GridRes());
	glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

	force.Use();
	state.AttachPosition(force, positionBufferName);
	state.AttachVelocity(force, velocityBufferName);

	glUniform1f(0, 0.15);

	glDispatchCompute(state.GridRes(), state.GridRes(), state.GridRes());
	glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
}
