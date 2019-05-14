#include "Shader.hpp"

#include "../Log/Logger.h"

#include <fstream>
#include <memory>

std::string ReadShader(const std::string& fileName)
{
	std::ifstream shaderStream(fileName);
	if (!shaderStream.is_open())
	{
		Logger::Warning() << "Couldn't open shader file " << fileName << '\n';
		return "";
	}

	//Copy the file to memory
	std::string shaderCode;
	shaderCode.assign(std::istreambuf_iterator<char>(shaderStream), std::istreambuf_iterator<char>());

	shaderStream.close();
	return shaderCode;
}

namespace GL {

std::string Shader::GetInfoLog() const
{
	GLint infoLogLength;
	glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &infoLogLength);

	std::unique_ptr<char> errorMessage(new char[infoLogLength]);
	glGetShaderInfoLog(shaderId, infoLogLength, NULL, errorMessage.get());

	return std::string(errorMessage.get());
}

bool Shader::FromFile(const std::string& fileName)
{
	return FromString(ReadShader(fileName));
}

bool Shader::FromString(const std::string& source)
{
	if(shaderId == 0)
	{
		Logger::Warning() << "Can't compile shader: shader name invalid.\n";
		return false;
	}

	const char* str1[1];
	str1[0] = source.c_str();
	glShaderSource( shaderId, 1, str1, NULL );

	// Compile shader
	glCompileShader( shaderId );

	// Get Compilation status
	GLint result = GL_FALSE;
	glGetShaderiv(shaderId, GL_COMPILE_STATUS, &result);

	return result == GL_TRUE;
}

}// namespace GL
