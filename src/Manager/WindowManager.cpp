#include "WindowManager.h"

#include "../Log/Logger.h"
#include "../Init/GlewInit.h"

#include <SDL2/SDL.h>

bool WindowManager::Init()
{
	return true;
}

bool WindowManager::SpawnWindow(const WindowInfo& info)
{
	if(!GlewInit::InitContext())
		return false;
	
	//Spawning Main Window
	mainWindow = SDL_CreateWindow(info.title.c_str(), info.x, info.y, info.w, info.h, info.flags);
	if(mainWindow == nullptr)
	{
		Logger::Error << "Couldn't create window: " << SDL_GetError() << '\n';
		return false;
	}
	
	// Creating OpenGL context
	oContext = SDL_GL_CreateContext(mainWindow);
    if (!oContext)
	{
		Logger::Error << "Couldn't create openGL context: " << SDL_GetError() << '\n';
        return false;
    }
	
	if(!GlewInit::InitGlew())
		return false;
	
	return true;
}

void WindowManager::DestroyWindow()
{
	SDL_DestroyWindow(mainWindow);
	SDL_GL_DeleteContext(oContext);
}