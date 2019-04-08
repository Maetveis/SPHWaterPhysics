#ifndef SPH_RENDER_HPP
#define SPH_RENDER_HPP

#include "../Helper/Texture.h"
#include "../Helper/Program.hpp"
#include "../Helper/VertexArray.hpp"

class SimulationState;

class SPHRender
{
private:
	SimulationState& state;
	GL::Texture distanceFieldTexture;

	GL::Program distanceFieldProgram;
	GL::Program raycastProgram;

	GL::VertexArray va;

 	void CompileShaders();
public:
	SPHRender(SimulationState& _state);

	~SPHRender() = default;

	void Render(float time);
};

#endif
