#include "UniformBuffer.hpp"

#include "Program.hpp"
#include "Buffer.hpp"

namespace GL {

void UniformBuffer::AttachBuffer(const Buffer& buffer)
{
	glBindBufferBase(GL_UNIFORM_BUFFER, bindingIndex, buffer.GetId());
}

void UniformBuffer::AttachBufferRange(const Buffer &buffer, GLuint offset, GLuint size)
{
	glBindBufferRange(GL_UNIFORM_BUFFER, bindingIndex, buffer.GetId(), offset, size);
}

void UniformBuffer::AttachToBlock(const Program& program, const GLuint index) const
{
	Logger::Debug() << "unform buffer bind:" << index << " -> " << bindingIndex <<  '\n';

	glUniformBlockBinding(program.Get(), index, bindingIndex);
}

}// namspace GL
