#include "Game.h"

#include "../Scene/InGameScene.h"
#include "../Scene/SPHWaterScene.hpp"

#include "../Log/Logger.h"
#include "../Init/SDLInit.h"
#include "../Model/WindowInfo.h"

#include "../Helper/StaticCounter.hpp"

#include <SDL2/SDL.h>
#include <iostream>


Game::Game() :
	running(true)
{
}

void Game::Run()
{
	// If something fails at init we quit
	if(Init())
	{
		//Main loop
		while(running)
		{
			HandleEvents();
			Update();
			Render();
			DelayFrameTime(timer.GetFrameStart(), targetFPS);
		}
	}
	Destroy();
}

void LogGLDebug(GLenum source, GLenum type, GLuint id,
	GLenum severity, GLsizei length, const GLchar* message, const void* userParam)
{
	switch (severity) {
		case GL_DEBUG_SEVERITY_HIGH:
			Logger::Error() << "Opengl: " << message << '\n';
			break;
		case GL_DEBUG_SEVERITY_MEDIUM:
			Logger::Warning() << "Opengl: " << message << '\n';
			break;
		case GL_DEBUG_SEVERITY_LOW:
			Logger::Warning() << "Opengl low warning: " << message << '\n';
			break;
		case GL_DEBUG_SEVERITY_NOTIFICATION:
			Logger::Info() << "Opengl notification: " << message << '\n';
			break;
	}
}

bool Game::Init()
{
	if (!SDLInit::Init())
		return false;

	if(!windowManager.Init())
		return false;

	WindowInfo info;
	info.SetTitle("SPH Water Simulation");
	windowManager.SpawnWindow(info);

	//Enable opengl debug output
	glDebugMessageCallback(LogGLDebug, nullptr);

	//Starting main Scene
	sceneManager.AttachGame(this);
	if(!sceneManager.ChangeScene(std::make_unique<SPHWaterScene>()))
	{
		Logger::Error() << "Failed initializing first scene. Exiting\n";
		return false;
	}

	//Starting delta timer
	timer.SetScaleFactor(1);
	timer.Start();

	return true;
}

void Game::HandleEvents()
{
	//This function is dipatching events to the actual scene
	SDL_Event event;
	while(SDL_PollEvent(&event))
	{
		switch(event.type)
		{
			case SDL_AUDIODEVICEADDED:
			case SDL_AUDIODEVICEREMOVED:
				sceneManager->OnAudioDevice(event.adevice);
				break;
			case SDL_CONTROLLERAXISMOTION:
				sceneManager->OnControllerAxis(event.caxis);
				break;
			case SDL_CONTROLLERBUTTONDOWN:
			case SDL_CONTROLLERBUTTONUP:
				sceneManager->OnControllerButton(event.cbutton);
				break;
			case SDL_CONTROLLERDEVICEADDED:
			case SDL_CONTROLLERDEVICEREMOVED:
			case SDL_CONTROLLERDEVICEREMAPPED:
				sceneManager->OnControllerDevice(event.cdevice);
				break;
			case SDL_DROPFILE:
			case SDL_DROPTEXT:
			case SDL_DROPBEGIN:
			case SDL_DROPCOMPLETE:
				sceneManager->OnDrop(event.drop);
				break;
			case SDL_FINGERMOTION:
			case SDL_FINGERDOWN:
			case SDL_FINGERUP:
				sceneManager->OnTouchFinger(event.tfinger);
				break;
			case SDL_KEYDOWN:
			case SDL_KEYUP:
				sceneManager->OnKeyboard(event.key);
				break;
			case SDL_JOYAXISMOTION:
				sceneManager->OnJoyAxis(event.jaxis);
				break;
			case SDL_JOYBALLMOTION:
				sceneManager->OnJoyBall(event.jball);
				break;
			case SDL_JOYHATMOTION:
				sceneManager->OnJoyHat(event.jhat);
				break;
			case SDL_JOYBUTTONDOWN:
			case SDL_JOYBUTTONUP:
				sceneManager->OnJoyButton(event.jbutton);
				break;
			case SDL_JOYDEVICEADDED:
			case SDL_JOYDEVICEREMOVED:
				sceneManager->OnJoyDevice(event.jdevice);
				break;
			case SDL_MOUSEMOTION:
				sceneManager->OnMouseMotion(event.motion);
				break;
			case SDL_MOUSEBUTTONDOWN:
			case SDL_MOUSEBUTTONUP:
				sceneManager->OnMouseButton(event.button);
				break;
			case SDL_MOUSEWHEEL:
				sceneManager->OnMouseWheel(event.wheel);
				break;
			case SDL_QUIT:
				sceneManager->OnQuit(event.quit);
				break;
			case SDL_USEREVENT:
				sceneManager->OnUser(event.user);
				break;
			case SDL_WINDOWEVENT:
				sceneManager->OnWindow(event.window);
				break;
			default:
				break;
		}
	}
}

void Game::Update()
{
	timer.Update();
	sceneManager->Update(timer.GetScaledTime());
}

void Game::Render()
{
	sceneManager->PrepareRender();
	sceneManager->Render();
	windowManager.PresentWindow();
}

void Game::DelayFrameTime(const unsigned frameStart, const unsigned short targetFPS)
{
	const unsigned targetLength = 1000 / targetFPS;
	const unsigned frameLenght = SDL_GetTicks() - frameStart;
	int delay = targetLength - frameLenght;
	if(delay > 0)
	{
		SDL_Delay(delay);
	}
}

void Game::Destroy()
{
	SDL_Quit();
}
