#include "InGameScene.h"

#include "../Log/Logger.h"
#include "../Main/Game.h"
#include "../Helper/UniformBuffer.hpp"
#include "../Model/Material/ColorFormat.hpp"
#include "../Model/ModelLoader.hpp"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform2.hpp>

#include <cmath>

bool InGameScene::Begin()
{
	glClearColor(0.3, 0., 0., 1.);
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glCullFace(GL_BACK);

	//glEnable(GL_BLEND);
	//glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//glEnable( GL_POINT_SMOOTH );
	//glPointSize( 10 );

	if(!LoadShaders())
	{
		Logger::Error << "Failed to load Shaders" << '\n';
		return false;
	}

	if(!LoadData())
	{
		Logger::Error << "Loading data failed" << '\n';
		return false;
	}

	Logger::Info << "RenderManager.Init() finished succesfully" << '\n'; //std::endl;

	return true;
}

bool InGameScene::LoadData()
{
	desc.AttachVertex(vertexBuffer.GetBuffer());
	desc.AttachIndex(indexBuffer.GetBuffer());

	ModelLoader loader(vertexBuffer, indexBuffer, program.GetMaterialParams());

	if(	!loader.ImportFile("../assets/cube.obj", cube)
		|| !loader.ImportFile("../assets/alfa.obj", car))
	{
		Logger::Error << "Model loading failed\n";
		return false;
	}

	return true;
}

bool InGameScene::LoadShaders()
{
	Logger::Debug << "UniformLocation(model): " << (modelID = program.Program().GetUniformLocation("model")) << '\n';

	program.SetProj(glm::perspective(45.0f, 640/360.0f, 0.01f, 500.0f));
	program.SetView(glm::lookAt(glm::vec3( 0.f,  1.f,  6.f), glm::vec3( 0,  0,  0), glm::vec3( 0,  1,  0)));

	program.Lights()[0] = Light(glm::vec3(0., 0., 1.), glm::vec3(.8, .7, .6), 30);

	program.Update();

	return true;
}

void InGameScene::Update(double deltaTime)
{
	time = SDL_GetTicks() / 300.0f;
}

void InGameScene::End()
{
}

void InGameScene::Pause()
{
}

void InGameScene::PrepareRender()
{
}

void InGameScene::Render()
{
	// Clear the screen and depth buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	program.Use();

	program.Update();

	matModel =
		glm::rotate(time * 0.002f, glm::vec3(0.f, 0.f, 1.f)) *
		glm::rotate(time * 0.3f, glm::vec3(0.f, 1.f, 0.f)) *
		glm::rotate(time * 0.1f, glm::vec3(1.f, 0.f, 0.f));

	glUniformMatrix4fv(modelID, 1, GL_FALSE, &(matModel[0][0]));

	//modelManager.Draw("iphone");
	desc.Bind();

	for(unsigned i = 0; i < cube.meshes.size(); ++i)
	{
		program.UseMaterial(cube.materials[i]);
		cube.meshes[i].Draw();
	}

	for(unsigned i = 0; i < car.meshes.size(); ++i)
	{
		program.UseMaterial(car.materials[i]);
		car.meshes[i].Draw();
	}

	program.Unuse();
}
void InGameScene::OnQuit(SDL_QuitEvent& event)
{
	Quit();
}

void InGameScene::OnWindow(SDL_WindowEvent& event)
{
	switch(event.event)
	{
		case SDL_WINDOWEVENT_RESIZED:
			glViewport(0, 0, event.data1, event.data2);
			program.SetProj(glm::perspective(45.0f, event.data1 / static_cast<float>(event.data2), 0.01f, 500.0f));
		default:
			break;
	}
}

void InGameScene::Quit()
{
	Logger::Debug << "Exit" << '\n';
	game->running = false;
}
void InGameScene::OnKeyboard(SDL_KeyboardEvent& event)
{
	switch(event.keysym.sym)
	{
		case SDLK_ESCAPE:
			Quit();
			break;
		default:
			Logger::Debug << "Pressed key with code: " << event.keysym.sym << '\n';
			break;
	}
}
