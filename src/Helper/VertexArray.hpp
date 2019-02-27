#ifndef VERTEX_ARRAY_HPP
#define VERTEX_ARRAY_HPP

#include <GL/glew.h>

#include "Buffer.hpp"

namespace GL {

class VertexArray
{
protected:
	GLuint id;
public:
	inline VertexArray()
	{
		glCreateVertexArrays(1, &id);
	}

	inline ~VertexArray()
	{
		glDeleteVertexArrays(1, &id);
	}

	inline void Bind() const
	{
		glBindVertexArray(id);
	}

	inline void AttachIndex(const Buffer& indexBuffer)
	{
		glVertexArrayElementBuffer(id, indexBuffer.GetId());
	}

	inline void EnableAttrib(GLuint index)
	{
		glEnableVertexArrayAttrib(id, index);
	}

	inline void FormatAttrib(GLuint index, GLint size, GLenum type, bool normalized, GLuint relativeOffset)
	{
		glVertexArrayAttribFormat(id, index, size, type, normalized, relativeOffset);
	}

	inline GLint GetId() const
	{
		return id;
	}

	inline static void AttribPointer(GLuint index, GLint size, GLenum type, bool normalized ,GLuint stride, void* offset)
	{
		glVertexAttribPointer(index, size, type, normalized, stride, offset);
	}
};

}// namespace GL

#endif //VERTEX_HPP
