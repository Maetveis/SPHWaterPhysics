#ifndef RENDER_EDGE_POINTS_HPP
#define RENDER_EDGE_POINTS_HPP

#include "../../Helper/Program.hpp"
#include "../../Helper/VertexArray.hpp"

class SimulationState;

class RenderEdgePoints
{
private:
	SimulationState& state;

	GL::Program renderProgram;

	GL::VertexArray va;

 	void CompileShaders();
public:
	RenderEdgePoints(SimulationState& _state);

	~RenderEdgePoints() = default;

	void Render(float time);
};

#endif
