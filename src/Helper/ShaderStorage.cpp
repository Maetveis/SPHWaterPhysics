#include "ShaderStorage.hpp"

#include "Program.hpp"
#include "Buffer.hpp"

namespace GL {

void ShaderStorage::AttachBuffer(const Buffer& buffer)
{
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, bindingIndex, buffer.GetId());
}

void ShaderStorage::AttachBufferRange(const Buffer &buffer, GLuint offset, GLuint size)
{
	glBindBufferRange(GL_SHADER_STORAGE_BUFFER, bindingIndex, buffer.GetId(), offset, size);
}

void ShaderStorage::AttachToBlock(const Program& program, const GLuint index) const
{
	//Logger::Debug() << "shader storage buffer bind:" << bindingIndex << " -> program" << program.Get() << ":"  << index <<  '\n';

	glShaderStorageBlockBinding(program.Get(), index, bindingIndex);
}

}// namspace GL
