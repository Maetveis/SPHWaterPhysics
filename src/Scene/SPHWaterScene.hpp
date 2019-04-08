#ifndef SPH_WATER_SCENE_HPP
#define SPH_WATER_SCENE_HPP

#include "Scene.h"

#include "../Helper/Program.hpp"
#include "../Helper/Buffer.hpp"
#include "../Helper/ShaderStorage.hpp"
#include "../Helper/VertexArray.hpp"
#include "../Helper/VertexArrayBinding.hpp"

#include "../SPHSimulation/SimulationState.hpp"

#include "../Program/GridProgram.hpp"
#include "../Program/SimulationProgram.hpp"
#include "../Program/SPHRender.hpp"

#include <GL/glew.h>

class SPHWaterScene : public Scene
{
private:
	GL::Program gravityProgram;

	GLint targetLocation;
	GLint dtLocation;

	SimulationState state;
	GridProgram grid;
	SimulationProgram simulation;
	SPHRender render;

	float time;
	float timeRemainder;

	static constexpr float stepTime = 0.016666666666;
public:
	SPHWaterScene() :
		state(64, 64, 64, 32),
		grid(state),
		simulation(state),
		render(state),
		time(0),
		timeRemainder(0)
	{
	}

	virtual bool Begin() override;
	virtual void End() override;
	virtual void Pause() override;
	virtual void Update(const double) override;

	virtual void PrepareRender() override;
	virtual void Render() override;

	virtual void OnQuit(SDL_QuitEvent& event) override;

	virtual void OnWindow(SDL_WindowEvent& event) override;

	virtual ~SPHWaterScene() override;
};

#endif //SPH_WATER_SCENE_HPP
