#ifndef SIMULATION_PROGRAM_HPP
#define SIMULATION_PROGRAM_HPP

#include "../Helper/Program.hpp"

class SimulationState;

class SimulationProgram
{
private:
	SimulationState& state;

	GL::Program pressure;

	void CompileShaders();

	static constexpr const char* pressureSource = "../shaders/Simulation/pressure.comp";
public:
	SimulationProgram(SimulationState& _state);

	void Run();
};

#endif //GRID_PROGRAM_HPP
