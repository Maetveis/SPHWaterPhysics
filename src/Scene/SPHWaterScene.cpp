#include "SPHWaterScene.hpp"

#include "../Helper/Shader.hpp"
#include "../Log/Logger.h"
#include "../Main/Game.h"

#include "../Program/GridProgram.hpp"

#include <vector>
#include <glm/vec3.hpp>

#include <GL/glew.h>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform2.hpp>

constexpr const char* positionBufferName = "positionBuffer";
constexpr const char* pressureBufferName = "pressureBuffer";
constexpr const char* velocityBufferName = "velocityBuffer";
constexpr const char* forceBufferName = "forceBuffer";
constexpr const char* indexBufferName = "indexBuffer";

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

	glPushDebugGroup(GL_DEBUG_SOURCE_APPLICATION, 0, sizeof("VSFS Shader") / sizeof(char), "VSFS Shader");

	if(!renderProgram.VsFsProgram("../shaders/Render/passthrough.vert", "../shaders/Render/passthrough.frag"))
	{
		Logger::Error << "Render Program linking failed: " << renderProgram.GetInfoLog() <<  '\n';
		return false;
	}

	glPopDebugGroup();

	glPushDebugGroup(GL_DEBUG_SOURCE_APPLICATION, 0, sizeof("uniforms") / sizeof(char), "uniforms");

	targetLocation = gravityProgram.GetUniformLocation("target");
	dtLocation = gravityProgram.GetUniformLocation("dt");

	projLocation = renderProgram.GetUniformLocation("proj");

	va.EnableAttrib(0);
	//va.FormatAttrib(0, sizeof(data[0]), GL_FLOAT, false, 0);
	//vb.BindAttrib(va, 0);

	//vb.BindBuffer(va, storageBuffer, 0, sizeof(data[0]));

	state.AttachParticleIndex(renderProgram, indexBufferName);

	glClearColor(0.3, 0., 0., 1.);
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_DEBUG_OUTPUT);

	GLint val;
	glGetIntegerv(GL_MAX_COMPUTE_SHARED_MEMORY_SIZE, &val);

	Logger::Debug << "Maximum size of shader local storage: " <<
		val << '\n';

	glPopDebugGroup();

	state.AttachPressure(renderProgram, pressureBufferName);
	state.AttachForce(gravityProgram, forceBufferName);

	glEnable(GL_PROGRAM_POINT_SIZE);

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

static double t = 0;

constexpr size_t groupX = 4;
constexpr size_t groupY = 4;
constexpr size_t groupZ = 4;

void SPHWaterScene::Update(const double delta)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	grid.Run();
	simulation.Run();

	t += delta;

	gravityProgram.Use();
	state.AttachPosition(gravityProgram, positionBufferName);
	state.AttachVelocity(gravityProgram, velocityBufferName);

	glUniform1f(dtLocation, static_cast<float>(delta));

	glm::vec3 target = glm::vec3(0. * glm::sin( 5 * t), 0. * glm::cos(3 * t), 0. * glm::sin( 4 * t));

	glUniform3fv(targetLocation, 1, reinterpret_cast<GLfloat*>(&target[0]));

	glDispatchCompute(state.ResX() / groupX, state.ResY() / groupY, state.ResZ() / groupZ);

	glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
}

 void SPHWaterScene::PrepareRender()
{
}

void SPHWaterScene::Render()
{
	//Render

	renderProgram.Use();
	state.AttachPosition(renderProgram, positionBufferName);

	va.Bind();

	glm::mat4 proj =
		glm::perspective(45.0f, 1280/720.0f, 0.01f, 500.0f) *
		glm::lookAt(glm::vec3( 0.f,  1.f,  2.f), glm::vec3( 0,  0,  0), glm::vec3( 0,  1,  0)) *
		//glm::rotate<float>(t * 0.002, glm::vec3(0.f, 0.f, 1.f)) *
		//glm::rotate<float>(t * 0.3, glm::vec3(0.f, 1.f, 0.f)) *
		glm::rotate<float>(t * 0.1, glm::vec3(0.f, 1.f, 0.f));

	glUniformMatrix4fv(projLocation, 1, GL_FALSE, reinterpret_cast<GLfloat*>(&proj[0][0]));

	glDrawArrays(GL_POINTS, 0, state.ResX() * state.ResY() * state.ResZ());
	glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

	va.UnBind();

	/*GLsync wait = glFenceSync(GL_SYNC_GPU_COMMANDS_COMPLETE, 0);
	glFlush();

	glWaitSync(wait, 0, GL_TIMEOUT_IGNORED);
	glClientWaitSync(wait, 0, 10000000000000);

	glDeleteSync(wait);*/
}

SPHWaterScene::~SPHWaterScene()
{

}
