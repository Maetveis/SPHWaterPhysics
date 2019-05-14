#ifndef SHADER_STORAGE_HPP
#define SHADER_STORAGE_HPP

#include <GL/glew.h>

#include "StaticCounter.hpp"
#include "../Log/Logger.h"

namespace GL
{
	class Program;
	class Buffer;
}

namespace GL {

class ShaderStorage
{
private:
	StaticCounter<GLuint, ShaderStorage> bindingIndex;
public:
	ShaderStorage()
	{
		Logger::Debug() << "Created shader storage buffer with binding index: " << bindingIndex.Get() << '\n';
	}

	void AttachToBlock(const Program& program, const GLuint index) const;

	void AttachBuffer(const Buffer& buffer);

	void AttachBufferRange(const Buffer& buffer, GLuint offset, GLuint size);

	GLuint GetBlockBinding() const
	{
		return bindingIndex;
	}
};

} //namespace GL

#endif //SHADER_STORAGE_HPP
