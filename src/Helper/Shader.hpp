#ifndef SHADER_HPP
#define SHADER_HPP

#include <GL/glew.h>
#include <utility>
#include <string>

namespace GL {

class Shader
{
private:
	GLuint shaderId;
public:
	inline Shader(GLenum shaderType)
	{
		shaderId = glCreateShader(shaderType);
	}

	inline ~Shader()
	{
		if(shaderId != 0)
			glDeleteShader(shaderId);
	}

	Shader() = delete;
	Shader(const Shader&) = delete;
	Shader& operator=(const Shader&) = delete;

	inline Shader(Shader&& other) :
		shaderId(other.shaderId)
	{
		other.shaderId = 0;
	}

	inline Shader& operator=(Shader&& other)
	{
		std::swap(shaderId, other.shaderId);
		return *this;
	}

	inline operator GLuint () const
	{
		return shaderId;
	}

	inline GLuint GetId() const
	{
		return shaderId;
	}

	std::string GetInfoLog() const;

	bool FromFile(const std::string& fileName);

	bool FromString(const std::string& source);
};

}// namespace GL

#endif
