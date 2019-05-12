#ifndef RENDER_SURFACE_HPP
#define RENDER_SURFACE_HPP

#include "../../Helper/Texture.h"
#include "../../Helper/Program.hpp"
#include "../../Helper/VertexArray.hpp"

#include "OrbiterCamera.hpp"
#include "Direction.hpp"

#include <memory>

class SimulationState;

class RenderSurface
{
private:
	SimulationState& state;
	std::unique_ptr<GL::Texture> distanceFieldTexture;

	GL::Program distanceFieldProgram;
	GL::Program raycastProgram;

	GL::VertexArray va;

	OrbiterCamera camera;

 	void CompileShaders();
	void DistanceField();
	void Raycast();
public:
	RenderSurface(SimulationState& _state);

	~RenderSurface() = default;

	void Render(float time);

	void SetDistanceTextureSize(unsigned length);

	void SetHorizontalDirection(Direction dir)
	{
		camera.SetHorizontalDirection(dir);
	}

	void SetVerticalDirection(Direction dir)
	{
		camera.SetVerticalDirection(dir);
	}

	glm::vec3 GetGravity()
	{
		return glm::vec3(camera.GetView() * glm::vec4(0, -1, 0, 0));
	}
};

#endif
