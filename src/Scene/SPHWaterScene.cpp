#include "SPHWaterScene.hpp"

#include "../Helper/Shader.hpp"
#include "../Log/Logger.h"
#include "../Main/Game.h"

#include "../Program/GridProgram.hpp"
#include "../Program/Render/Direction.hpp"

#include <cmath>
#include <GL/glew.h>

static constexpr const char* positionBufferName = "positionBuffer";
static constexpr const char* densityBufferName  = "densityBuffer";
static constexpr const char* velocityBufferName = "velocityBuffer";
static constexpr const char* forceBufferName = "forceBuffer";
//static constexpr const char* indexBufferName = "indexBuffer";

static constexpr const unsigned DtLocation = 0;

void SPHWaterScene::OnWindow(SDL_WindowEvent& event)
{
	switch(event.event)
	{
		case SDL_WINDOWEVENT_RESIZED:
			glViewport(0, 0, event.data1, event.data2);
		default:
			break;
	}
}

bool SPHWaterScene::Begin()
{
	glPushDebugGroup(GL_DEBUG_SOURCE_APPLICATION, 0, sizeof("Scene Init") / sizeof(char), "Scene Init");

	glPushDebugGroup(GL_DEBUG_SOURCE_APPLICATION, 0, sizeof("Compute Shader") / sizeof(char), "Compute Shader");

	GL::Shader gravityShader(GL_COMPUTE_SHADER);

	if(!gravityShader.FromFile("../shaders/basic.comp"))
	{
		Logger::Error << "Gravity Shader compilation failed: " << gravityShader.GetInfoLog() <<  '\n';
		return false;
	}

	gravityProgram.AttachShader(gravityShader);

	if(!gravityProgram.Link())
	{
		Logger::Error << "Gravity Program linking failed: " << gravityProgram.GetInfoLog() <<  '\n';
		return false;
	}

	glPopDebugGroup();

	glPushDebugGroup(GL_DEBUG_SOURCE_APPLICATION, 0, sizeof("uniforms") / sizeof(char), "uniforms");

	targetLocation = gravityProgram.GetUniformLocation("target");
	dtLocation = gravityProgram.GetUniformLocation("dt");

	glClearColor(1., 1., 1., 1.);
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_DEBUG_OUTPUT);

	GLint val;
	glGetIntegerv(GL_MAX_COMPUTE_SHARED_MEMORY_SIZE, &val);

	Logger::Debug << "Maximum size of shader local storage: " <<
		val << '\n';

	glPopDebugGroup();

	state.AttachForce(gravityProgram, forceBufferName);
	state.AttachDensity(gravityProgram, densityBufferName);

	glEnable(GL_PROGRAM_POINT_SIZE);

	glEnable (GL_BLEND);
	glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glPopDebugGroup();

	return true;
}

void SPHWaterScene::OnQuit(SDL_QuitEvent& event)
{
	game->running = false;
}

void SPHWaterScene::End()
{

}
#include "../Program/Render/Direction.hpp"
void SPHWaterScene::Pause()
{

}

constexpr size_t groupX = 4;
constexpr size_t groupY = 4;
constexpr size_t groupZ = 4;

void SPHWaterScene::Update(const double delta)
{
	timeRemainder += delta;

	if(!paused && timeRemainder >= stepTime)
	{
		time += stepTime;
		timeRemainder = std::fmod(timeRemainder, stepTime);

		grid.Run();
		simulation.Run();

		gravityProgram.Use();
		state.AttachPosition(gravityProgram, positionBufferName);
		state.AttachVelocity(gravityProgram, velocityBufferName);

		glUniform1f(DtLocation, stepTime / 2);

		glm::vec3 grav = render.GetGravity();
		glUniform3fv(1, 1, reinterpret_cast<const GLfloat*>(&grav[0]));

		glDispatchCompute(state.ResX() / groupX, state.ResY() / groupY, state.ResZ() / groupZ);

		glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
	}
}

 void SPHWaterScene::PrepareRender()
{
}

void SPHWaterScene::Render()
{
	render.Render(time);
}

void SPHWaterScene::OnKeyboard(SDL_KeyboardEvent& event)
{
	switch(event.keysym.sym)
	{
		case SDLK_ESCAPE:
			game->running = false;
			break;
		case 'k':
			if(event.state == SDL_RELEASED)
				paused = !paused;
			break;
		case 'w':
			if(event.state == SDL_PRESSED)
				render.SetVerticalDirection(Direction::Forward);
			else
				render.SetVerticalDirection(Direction::Stop);
			break;
		case 's':
			if(event.state == SDL_PRESSED)
				render.SetVerticalDirection(Direction::Backward);
			else
				render.SetVerticalDirection(Direction::Stop);
			break;
		case 'a':
			if(event.state == SDL_PRESSED)
				render.SetHorizontalDirection(Direction::Forward);
			else
				render.SetHorizontalDirection(Direction::Stop);
			break;
		case 'd':
			if(event.state == SDL_PRESSED)
				render.SetHorizontalDirection(Direction::Backward);
			else
				render.SetHorizontalDirection(Direction::Stop);
			break;
		default:
			Logger::Debug << "Pressed key with code: " << event.keysym.sym << '\n';
			break;
	}
}

SPHWaterScene::~SPHWaterScene()
{

}
