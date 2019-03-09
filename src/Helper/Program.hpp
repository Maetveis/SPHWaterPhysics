#ifndef PROGRAM_H
#define PROGRAM_H

#include <GL/glew.h>
#include <string>
#include <memory>

#include "Shader.hpp"

namespace GL
{

class Program
{
private:
	GLuint programID;
public:
	Program()
	{
		programID = glCreateProgram();
	}

	inline void CreateName()
	{
	}

	inline explicit Program(GLuint p) :
		programID(p)
	{
	}

	inline ~Program()
	{
		Destroy();
	}

	inline GLuint Get() const
	{
		return programID;
	}

	inline GLuint operator*() const
	{
		return programID;
	}

	inline operator bool ()
	{
		GLint result;
		glGetProgramiv(programID, GL_LINK_STATUS, &result);
		return result == GL_TRUE;
	}

	inline bool operator ! ()
	{
		GLint result;
		glGetProgramiv(programID, GL_LINK_STATUS, &result);
		return result != GL_TRUE;
	}

	inline GLuint GetUniformLocation(const char* name) const
	{
		return glGetUniformLocation(programID, name);
	}

	inline GLuint GetUniformBlockIndex(const char* name) const
	{
		return glGetUniformBlockIndex(programID, name);
	}

	inline GLuint GetShaderStorageBlockIndex(const char* name) const
	{
		return glGetProgramResourceIndex(programID, GL_SHADER_STORAGE_BLOCK, name);
	}

	inline void AttachShader(const Shader& shader)
	{
		glAttachShader(programID, shader.GetId());
	}

	inline bool Link()
	{
		GLint result = GL_FALSE;

		glLinkProgram(programID);
		glGetProgramiv(programID, GL_LINK_STATUS, &result);

		return result == GL_TRUE;
	}

	inline void Use() const
	{
		glUseProgram(programID);
	}

	inline void Unuse() const
	{
		glUseProgram(0);
	}

	inline void Destroy()
	{
		glDeleteProgram(programID);
	}

	inline std::string GetInfoLog()
	{
		GLint infoLogLength;
		glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &infoLogLength);

		std::unique_ptr<char> errorMessage(new char[infoLogLength]);
		glGetProgramInfoLog(programID, infoLogLength, NULL, errorMessage.get());

		return std::string(errorMessage.get());
	}

	bool VsFsProgram( 	const std::string& vertexShaderName,
						const std::string& fragmentShaderName);
};

}// namespace GL;

#endif //PROGRAM_H
