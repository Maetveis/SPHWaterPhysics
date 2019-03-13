#ifndef GRID_PROGRAM_HPP
#define GRID_PROGRAM_HPP

#include "../Helper/Program.hpp"

class SimulationState;

class GridProgram
{
private:
	SimulationState& state;

	GL::Program count;
	GL::Program offset;
	GL::Program superBlock;
	GL::Program finalize;
	GL::Program scatter;

	void CompileShaders();

	static constexpr const char* countSource = "../shaders/Grid/count.comp";
	static constexpr const char* offsetSource = "../shaders/Grid/offset.comp";
	static constexpr const char* superBlockSource = "../shaders/Grid/superBlock.comp";
	static constexpr const char* scatterSource = "../shaders/Grid/scatter.comp";
	static constexpr const char* finalizeSource = "../shaders/Grid/finalize.comp";
public:
	GridProgram(SimulationState& _state);

	void Run();
};

#endif //GRID_PROGRAM_HPP
