#include "SPHWaterScene.hpp"

#include "../Helper/Shader.hpp"
#include "../Log/Logger.h"
#include "../Main/Game.h"

#include "../Program/GridProgram.hpp"

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

	glClearColor(0., 0., 0., 1.);
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

void SPHWaterScene::Pause()
{

}

constexpr size_t groupX = 4;
constexpr size_t groupY = 4;
constexpr size_t groupZ = 4;

void SPHWaterScene::Update(const double delta)
{
	timeRemainder += delta;

	if(timeRemainder >= stepTime)
	{
		time += stepTime;
		timeRemainder = std::fmod(timeRemainder, stepTime);

		grid.Run();
		simulation.Run();

		gravityProgram.Use();
		state.AttachPosition(gravityProgram, positionBufferName);
		state.AttachVelocity(gravityProgram, velocityBufferName);

		glUniform1f(DtLocation, stepTime / 2);

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

	/*GLsync wait = glFenceSync(GL_SYNC_GPU_COMMANDS_COMPLETE, 0);
	glFlush();

	glWaitSync(wait, 0, GL_TIMEOUT_IGNORED);
	glClientWaitSync(wait, 0, 10000000000000);

	glDeleteSync(wait);*/
}

SPHWaterScene::~SPHWaterScene()
{

}
