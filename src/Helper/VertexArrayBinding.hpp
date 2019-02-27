#ifndef VERTEX_ARRAY_BINDING
#define VERTEX_ARRAY_BINDING

#include "VertexArray.hpp"

#include "StaticCounter.hpp"

#include <GL/glew.h>

namespace GL {

class VertexArrayBinding
{
private:
	StaticCounter<GLuint, VertexArrayBinding> binding;
public:
	inline void BindAttrib(const VertexArray& va, GLuint attribIndex)
	{
		glVertexArrayAttribBinding(va.GetId(), attribIndex, binding);
	}

	inline void BindBuffer(const VertexArray& va, const Buffer& buffer, GLintptr offset, GLsizei stride)
	{
		glVertexArrayVertexBuffer(va.GetId(), binding, buffer.GetId(), offset, stride); // Check this line if it doesnt work
	}
};

} //namespace GL

#endif //VERTEX_ARRAY_BINDING
