#ifndef RENDER_POINTS_HPP
#define RENDER_POINTS_HPP

#include "../../Helper/Program.hpp"
#include "../../Helper/VertexArray.hpp"

class SimulationState;

class RenderPoints
{
private:
	SimulationState& state;

	GL::Program renderProgram;

	GL::VertexArray va;

 	void CompileShaders();
public:
	RenderPoints(SimulationState& _state);

	~RenderPoints() = default;

	void Render(float time);
};

#endif
