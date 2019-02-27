#ifndef WINDOW_MANAGER_H
#define WINDOW_MANAGER_H

#include "../Model/WindowInfo.h"

#include <SDL2/SDL.h>

class WindowManager
{
public:
	bool Init();
	bool SpawnWindow(const WindowInfo&);
	void DestroyWindow();
	
	void PresentWindow()
	{
		SDL_GL_SwapWindow(mainWindow);
	}
	
	~WindowManager()
	{
		DestroyWindow();
	}
private:
	SDL_Window* mainWindow;
	SDL_GLContext oContext;
};

#endif //WINDOW_MANAGER_H