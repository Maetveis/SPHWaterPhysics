#ifndef RENDER_SURFACE_HPP
#define RENDER_SURFACE_HPP

#include "../../Helper/Texture.h"
#include "../../Helper/Program.hpp"
#include "../../Helper/VertexArray.hpp"

class SimulationState;

class RenderSurface
{
private:
	SimulationState& state;
	GL::Texture distanceFieldTexture;

	GL::Program distanceFieldProgram;
	GL::Program raycastProgram;

	GL::VertexArray va;

 	void CompileShaders();
public:
	RenderSurface(SimulationState& _state);

	~RenderSurface() = default;

	void Render(float time);
};

#endif
