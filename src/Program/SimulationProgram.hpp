#ifndef SIMULATION_PROGRAM_HPP
#define SIMULATION_PROGRAM_HPP

#include "../Helper/Program.hpp"

class SimulationState;

class SimulationProgram
{
private:
	SimulationState& state;

	GL::Program pressure;
	GL::Program force;

	void CompileShaders();

	static constexpr const char* pressureSource = "../shaders/Simulation/pressure.comp";
	static constexpr const char* forceSource = "../shaders/Simulation/force.comp";
public:
	SimulationProgram(SimulationState& _state);

	void Run();
};

#endif //GRID_PROGRAM_HPP
