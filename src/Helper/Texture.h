#ifndef TEXTURE_H
#define TEXTURE_H

#include <GL/glew.h>
#include <string>

struct SDL_Surface;

namespace GL {

class Texture
{
public:
	Texture()
	{
	}

	inline explicit Texture(GLenum target)
	{
		glCreateTextures(target, 1, &textureID);
	}

	Texture(const Texture&) = delete;

	Texture& operator=(const Texture&) = delete;

	void FromFile(GLenum target, const std::string fileName);

	void FromSurface(const SDL_Surface* surface, GLint level = 0);

	inline void SetMinFilter(GLenum filter)
	{
		glTextureParameteri(textureID, GL_TEXTURE_MIN_FILTER, filter);
	}

	inline void SetMagFilter(GLenum filter)
	{
		glTextureParameteri(textureID, GL_TEXTURE_MAG_FILTER, filter);
	}

	inline void GenerateMipmap()
	{
		glGenerateTextureMipmap(textureID);
	}

	inline GLuint GetId() const
	{
		return textureID;
	}

	inline void Bind(GLuint unit)
	{
		glBindTextureUnit(GL_TEXTURE0 + unit, textureID);
	}

	inline ~Texture()
	{
		glDeleteTextures(1, &textureID);
	}
private:
	GLuint textureID;
};

} //namespace GL

#endif //TEXTURE_H
