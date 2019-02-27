#ifndef SET_UNIFORM_HPP
#define SET_UNIFORM_HPP

#include <GL/glew.h>

namespace GL
{

template <typename T>
void SetUniform(GLint location, const T& data)
{

}

template <typename T>
void SetUniformImpl(const T& data)
{
	static_assert(data, "asd");
}

#define GL_SET_UNIFORM_IMPL( T ) \ template <typename T, >
void SetUniform

} //namespace GL

#endif //SET_UNIFORM_HPP
