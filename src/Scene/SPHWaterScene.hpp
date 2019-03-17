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

#include <GL/glew.h>

class SPHWaterScene : public Scene
{
private:
	GL::Program gravityProgram;
	GL::Program renderProgram;
	//GL::Buffer storageBuffer;
	//GL::ShaderStorage storage;

	GL::VertexArray va;
	GL::VertexArrayBinding vb;

	GLint targetLocation;
	GLint dtLocation;

	GLint projLocation;

	SimulationState state;
	GridProgram grid;
	SimulationProgram simulation;
public:
	SPHWaterScene() :
		state(64, 64, 64, 32),
		grid(state),
		simulation(state)
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
