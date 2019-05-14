#include "SDLInit.h"

#include "../Log/Logger.h"

#include <SDL2/SDL.h>

bool SDLInit::Init()
{
	//Initializing SDL
	if(SDL_Init(SDL_INIT_EVENTS | SDL_INIT_VIDEO) < 0)
	{
		Logger::Error() << "Couldn't initialize SDL: " << SDL_GetError() << '\n';
		return false;
	}
	
	return true;
}