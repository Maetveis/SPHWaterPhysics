#include "WindowInfo.h"

#include <SDL2/SDL.h>

WindowInfo::WindowInfo() : 
	title(""),
	x(SDL_WINDOWPOS_UNDEFINED),
	y(SDL_WINDOWPOS_UNDEFINED),
	w(640),
	h(360),
	flags(SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE)
{
}


void WindowInfo::SetTitle(const std::string& t)
{
	title = t;
}

void WindowInfo::SetPos(int X, int Y)
{
	x = X;
	y = Y;
}

void WindowInfo::SetSize(int W, int H)
{
	w = W;
	h = H;
}

void WindowInfo::SetOpenGL(bool val)
{
	if(val)
	{
		flags |= SDL_WINDOW_OPENGL;
	} else {
		flags &= SDL_WINDOW_OPENGL ^ 1;
	}
}

void WindowInfo::SetVisible(bool val)
{
	if(val)
	{
		flags &= SDL_WINDOW_HIDDEN ^ 1;
	} else {
		flags |= SDL_WINDOW_HIDDEN;
	}
}

void WindowInfo::SetResizable(bool val)
{
	if(val)
	{
		flags |= SDL_WINDOW_RESIZABLE;
	} else {
		flags &= SDL_WINDOW_RESIZABLE ^ 1;
	}
}