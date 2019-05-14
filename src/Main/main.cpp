#include "Game.h"

#include <SDL2/SDL_main.h>

#include "../Log/Logger.h"

int main(int argc, char* args[])
{

	if(argc > 1 && std::string(args[1]) == "-d")
		Logging::Settings::SetLevel(Logging::Level::Debug);
	else
		Logging::Settings::SetLevel(Logging::Level::Error);

	Game game;
	game.Run();

	return 0;
}
