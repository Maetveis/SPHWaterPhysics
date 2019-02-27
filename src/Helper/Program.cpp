#include "Program.hpp"

#include "Shader.hpp"

#include "../Log/Logger.h"

#include <fstream>

namespace GL {

bool Program::VsFsProgram( const std::string& vertexShaderName,
	const std::string& fragmentShaderName)
{
	Shader vertexShader(GL_VERTEX_SHADER);
	Shader fragmentShader(GL_FRAGMENT_SHADER);

	bool valid = true;
	if(!vertexShader.FromFile(vertexShaderName))
	{
		Logger::Error << "Vertex Shader Compilation failed:\n"
			<< vertexShader.GetInfoLog() << '\n';
		valid = false;
	}

	if(!fragmentShader.FromFile(fragmentShaderName))
	{
		Logger::Error << "Fragment Shader Compilation failed:\n"
			<< fragmentShader.GetInfoLog() << '\n';
		valid = false;
	}

	if(!valid)
	{
		Logger::Error << "Vertex Fragment program creation failed: Shader compilation failed\n";
		return false;
	}

	AttachShader(vertexShader);
	AttachShader(fragmentShader);
	if(!Link())
	{
		Logger::Error << "Vertex Fragment program creation failed: Linking failed:\n"
			<< GetInfoLog() << '\n';
	}

	return true;
}

}// namespace GL
