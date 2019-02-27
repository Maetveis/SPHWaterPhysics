#ifndef TYPE_ENUM_HPP
#define TYPE_ENUM_HPP

#include <GL/glew.h>

namespace {
	template<typename T>
	struct AlwaysFalse
	{
		static constexpr bool value = false;
	};
}

namespace GL
{
	template<typename T>
	struct TypeEnum
	{
		static_assert(AlwaysFalse<T>::value, "Trying to get GLenum of Unkown Type\n");
	};

#define GL_TYPE_ENUM_DEF(Type, Enum) \
template<> \
struct TypeEnum<Type> \
{ \
	static constexpr GLenum value = Enum; \
	constexpr operator GLenum() { return value; } \
};

	GL_TYPE_ENUM_DEF(GLint, 	GL_INT)
	GL_TYPE_ENUM_DEF(GLuint,	GL_UNSIGNED_INT)

	GL_TYPE_ENUM_DEF(GLshort, 	GL_SHORT)
	GL_TYPE_ENUM_DEF(GLushort,	GL_UNSIGNED_SHORT)

	GL_TYPE_ENUM_DEF(GLbyte, 	GL_BYTE)
	GL_TYPE_ENUM_DEF(GLubyte,	GL_UNSIGNED_BYTE)


	GL_TYPE_ENUM_DEF(GLfloat, 	GL_FLOAT)
	GL_TYPE_ENUM_DEF(GLdouble, 	GL_DOUBLE)

#undef GL_TYPE_ENUM_DEF
}

#endif //TYPE_ENUM_HPP
