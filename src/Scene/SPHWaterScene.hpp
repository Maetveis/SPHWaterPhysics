#ifndef SPH_WATER_SCENE_HPP
#define SPH_WATER_SCENE_HPP

#include "Scene.h"

#include "../Helper/Program.hpp"
#include "../Helper/ShaderStorage.hpp"

#include "../SPHSimulation/SimulationState.hpp"

#include "../Program/GridProgram.hpp"
#include "../Program/SimulationProgram.hpp"

#include "../Program/Render/RenderSurface.hpp"
#include "../Program/Render/RenderPoints.hpp"
#include "../Program/Render/RenderEdgePoints.hpp"

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
	RenderSurface render;

	float time;
	float timeRemainder;

	bool paused;

	static constexpr float stepTime = 0.016666666666;
public:
	SPHWaterScene() :
		state(32, 64, 64, 20),
		grid(state),
		simulation(state),
		render(state),
		time(0),
		timeRemainder(0),
		paused(false)
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
	void OnKeyboard(SDL_KeyboardEvent& event) override;

	virtual ~SPHWaterScene() override;
};

#endif //SPH_WATER_SCENE_HPP
