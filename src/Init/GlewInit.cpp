#include "GlewInit.h"

#include "../Log/Logger.h"

#include <GL/glew.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>

bool GlewInit::InitContext()
{
	// Setting up SDL_OPENGL Attributes
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 5);

	// Color buffers size
    SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE, 32);
    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);

	// Double-buffering
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	// Depth-buffer size
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

	// Anti-aliasing
	// SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
	// SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4);

	// V-sync on
	//SDL_GL_SetSwapInterval(1);

	return true;
}

bool GlewInit::InitGlew()
{
	// Starting Glew
	GLenum error = glewInit();
	if ( error != GLEW_OK )
	{
		Logger::Error << "Glew Init failed: " << glewGetErrorString(error) << '\n';
		return false;
	}

	Logger::Info << "Opengl version: " << glGetString(GL_VERSION) << '\n';

	return true;
}
