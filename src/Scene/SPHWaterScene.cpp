#include "SPHWaterScene.hpp"

#include "../Helper/Shader.hpp"
#include "../Log/Logger.h"
#include "../Main/Game.h"

#include <vector>
#include <glm/vec3.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform2.hpp>

struct alignas(16) alignedData
{
	glm::vec3 pos;
	alignedData(const glm::vec3 _pos) :
		pos(_pos)
	{
	}

	alignedData(float c1, float c2, float c3) :
		pos(c1, c2, c3)
	{
	}
};

constexpr size_t resX = 128;
constexpr size_t resY = 128;
constexpr size_t resZ = 128;

std::vector<alignedData> array()
{
	std::vector<alignedData> data;

	data.reserve(resX * resY * resZ);

	const float multX = 2. / resX;
	const float multY = 2. / resY;
	const float multZ = 2. / resZ;

	for(size_t x = 0; x < resX; ++x)
	{
		for(size_t y = 0; y < resY; ++y)
		{
			for(size_t z = 0; z < resZ; ++z)
			{
				data.emplace_back(x * multX - 1, y * multY - 1, z * multZ - 1);
			}
		}
	}

	return data;
}

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

	if(!renderProgram.VsFsProgram("../shaders/main.vert", "../shaders/main.frag"))
	{
		Logger::Error << "Render Program linking failed: " << renderProgram.GetInfoLog() <<  '\n';
		return false;
	}

	targetLocation = gravityProgram.GetUniformLocation("target");
	dtLocation = gravityProgram.GetUniformLocation("dt");

	projLocation = renderProgram.GetUniformLocation("proj");

	storage.AttachBuffer(storageBuffer);
	storage.AttachToBlock(gravityProgram, 0);

	auto data = array();

	storageBuffer.BufferData(data, GL_DYNAMIC_DRAW);

	va.EnableAttrib(0);
	va.FormatAttrib(0, sizeof(data[0]), GL_FLOAT, false, 0);
	vb.BindAttrib(va, 0);

	vb.BindBuffer(va, storageBuffer, 0, sizeof(data[0]));

	glClearColor(0.3, 0., 0., 1.);
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);

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

void SPHWaterScene::Update(const double delta)

{
	t += delta;

	gravityProgram.Use();

	glUniform1f(dtLocation, static_cast<float>(delta));

	glm::vec3 target = glm::vec3(glm::sin( 5 * t), glm::cos(3 * t), glm::sin( 4 * t) * glm::cos(7 * t));

	glUniform3fv(targetLocation, 1, reinterpret_cast<GLfloat*>(&target[0]));

	glDispatchCompute(resX / 8, resY / 8, resZ / 8);
	va.Bind();

	glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT | GL_VERTEX_ATTRIB_ARRAY_BARRIER_BIT);
}

 void SPHWaterScene::PrepareRender()
{

}

void SPHWaterScene::Render()
{
	//Render

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	va.Bind();
	renderProgram.Use();

	glm::mat4 proj = glm::perspective(45.0f, 640/360.0f, 0.01f, 500.0f) * glm::lookAt(glm::vec3( 0.f,  1.f,  2.f), glm::vec3( 0,  0,  0), glm::vec3( 0,  1,  0));
	glUniformMatrix4fv(projLocation, 1, GL_FALSE, reinterpret_cast<GLfloat*>(&proj[0][0]));

	glDrawArrays(GL_POINTS, 0, resX * resY * resZ);
}

SPHWaterScene::~SPHWaterScene()
{

}
