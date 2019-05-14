#include "Texture.h"

#include "../Log/Logger.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

namespace GL {

void Texture::FromFile(GLenum target, const std::string fileName)
{
	SDL_Surface* loaded_img = IMG_Load(fileName.c_str());
	if(loaded_img == nullptr)
	{
		Logger::Error() << "Failed to load Image: " << fileName << '\n';
		return;
	}

	FromSurface(loaded_img);
	SDL_FreeSurface(loaded_img);
}

void Texture::FromSurface(const SDL_Surface* surface, GLint level)
{
	GLint img_mode;

	#if SDL_BYTEORDER == SDL_LIL_ENDIAN
	if (surface->format->BytesPerPixel == 4)
		img_mode = GL_RGBA;
	else
		img_mode = GL_RGB;
	#else
	if (loaded_img->format->BytesPerPixel == 4)
		img_mode = GL_BGRA;
	else
		img_mode = GL_BGR;
	#endif

	glTextureStorage2D(textureID, 1, GL_RGBA, surface->w, surface->h);
	glTextureSubImage2D(textureID, level, 0, 0, surface->w, surface->h, img_mode, GL_UNSIGNED_BYTE, surface->pixels);
}

} //namespace GL
