#ifndef TYPE_TRAITS_HPP
#define TYPE_TRAITS_HPP

#define GL_UNIFORM_WRAP_GLM

#include <GL/glew.h>
#include <vector>

namespace {
	template <typename T>
	struct AlwaysFalse
	{
		static constexpr bool value = false;
	};
}

namespace GL {


template <typename T, unsigned short R>
struct VectorType
{
	static_assert(	std::is_same<T, GLuint>::value
						| std::is_same<T, GLint>::value
				 		| std::is_same<T, GLfloat>::value
					, "Trying to uniform wrap vector with unkown underlying type");

	static_assert(R >= 1 && R <= 4, "Trying to uniform wrap vector with too many/few coordinates");

	using base_type = T;

	T const * begin;
	static constexpr GLsizei count = static_cast<GLsizei>(R);

	inline VectorType(T const * _begin) :
		begin(_begin)
	{
	}
};

template <typename T, unsigned short R, unsigned short C>
struct MatrixType
{
	static_assert(	std::is_same<T, GLuint>::value
						| std::is_same<T, GLint>::value
						| std::is_same<T, GLfloat>::value
					, "Trying to uniform wrap vector with unkown underlying type");

	static_assert(R >= 2 && R <=4, "Trying to uniform wrap matrix with too many/few rows");
	static_assert(C >= 2 && C <=4, "Trying to uniform wrap matrix with too many/few	struct columns");

	static constexpr unsigned short Rows = R;
	static constexpr unsigned short Columns = C;

	using base_type = T;

	T const * begin;
	static constexpr GLsizei count = static_cast<GLsizei>(R * C);

	inline MatrixType(T const * _begin) :
		begin(_begin)
	{
	}
};

template <typename T>
struct ArrayType
{
	static_assert(	std::is_same<T, GLuint>::value
						| std::is_same<T, GLint>::value
						| std::is_same<T, GLfloat>::value
					, "Trying to uniform wrap array with unkown underlying type");

	using base_type = T;

	T const * begin;
	GLsizei count;

	inline ArrayType(T const * _begin, GLsizei _count) :
		begin(_begin),
		count(_count)
	{
	}
};

template <typename T, unsigned char R>
struct ArrayType< VectorType<T, R> >
{
	T const * begin;
	GLsizei count;

	using base_type = T;

	inline ArrayType(T const * _begin, GLsizei _count) :
		begin(_begin),
		count(_count)
	{
	}
};

template <typename T, unsigned short R, unsigned short C>
struct ArrayType< MatrixType<T, R, C> >
{
	T const * begin;
	GLsizei count;

	using base_type = T;

	inline ArrayType(T const * _begin, GLsizei _count) :
		begin(_begin),
		count(_count)
	{
	}
};

template <typename T>
struct WrapUniform
{
	static_assert(AlwaysFalse<T>::value, "Trying to wrap an unkown type to uniform");
};

template <>
struct WrapUniform<float>
{
	using type = GLfloat;
	using base_type = type;

	inline static type Wrap( float a )
	{
		return static_cast<type>(a);
	}
};

template <>
struct WrapUniform<unsigned int>
{
	using type = GLuint;
	using base_type = type;

	inline static type Wrap( unsigned int a )
	{
		return static_cast<type>(a);
	}
};

template <>
struct WrapUniform<int>
{
	using type = GLint;
	using base_type = type;

	inline static type Wrap( int a )
	{
		return static_cast<type>(a);
	}
};


template <typename T, typename... Args>
struct WrapUniform< std::vector<T, Args...> >
{
	using type = ArrayType< typename WrapUniform< T >::type >;
	using base_type = typename type::base_type;

	inline static type Wrap( const std::vector<T, Args...>& vec)
	{
		return type(reinterpret_cast<const base_type *>(vec.data()), vec.size());
	}
};

template <typename T, unsigned int size>
struct WrapUniform< T[size] >
{
	using type = ArrayType< typename WrapUniform< T >::type >;
	using base_type = typename type::base_type;

	inline static type Wrap( const T* const data )
	{
		return type(data, size);
	}
};

template<typename T>
auto UniformWrap(const T& data)
{
	return WrapUniform<T>::Wrap(data);
}

} //namespace GL;

#ifdef GL_UNIFORM_WRAP_GLM

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat2x2.hpp>
#include <glm/mat2x3.hpp>
#include <glm/mat2x4.hpp>
#include <glm/mat3x2.hpp>
#include <glm/mat3x3.hpp>
#include <glm/mat3x4.hpp>
#include <glm/mat4x2.hpp>
#include <glm/mat4x3.hpp>
#include <glm/mat4x4.hpp>

namespace GL {

#define GL_UNIFORM_VECTOR_WRAP(TYPE, ROWS, TOTYPE) template <> \
struct WrapUniform< TYPE > \
{ \
	using type = VectorType< TOTYPE , ROWS >; \
	inline static type Wrap( const TYPE &data ) \
	{ \
		return type(reinterpret_cast< const TOTYPE * const>(&data)); \
	} \
};

#define GL_UNIFORM_MATRIX_WRAP(TYPE, ROWS, COLS, TOTYPE) template <> \
struct WrapUniform< TYPE > \
{ \
	using type = MatrixType< TOTYPE , ROWS, COLS >; \
	inline static type Wrap( const TYPE &data ) \
	{ \
		return type(reinterpret_cast< const TOTYPE * const>(&data)); \
	} \
};

GL_UNIFORM_VECTOR_WRAP(glm::vec2, 2, float)
GL_UNIFORM_VECTOR_WRAP(glm::vec3, 3, float)
GL_UNIFORM_VECTOR_WRAP(glm::vec4, 4, float)

GL_UNIFORM_MATRIX_WRAP(glm::mat2x2, 2, 2, float)
GL_UNIFORM_MATRIX_WRAP(glm::mat2x3, 2, 3, float)
GL_UNIFORM_MATRIX_WRAP(glm::mat2x4, 2, 4, float)
GL_UNIFORM_MATRIX_WRAP(glm::mat3x2, 3, 2, float)
GL_UNIFORM_MATRIX_WRAP(glm::mat3x3, 3, 3, float)
GL_UNIFORM_MATRIX_WRAP(glm::mat3x4, 3, 4, float)
GL_UNIFORM_MATRIX_WRAP(glm::mat4x2, 4, 2, float)
GL_UNIFORM_MATRIX_WRAP(glm::mat4x3, 4, 3, float)
GL_UNIFORM_MATRIX_WRAP(glm::mat4x4, 4, 4, float)

#undef GL_UNIFORM_MATRIX_WRAP
#undef GL_UNIFORM_VECTOR_WRAP

} //namespace GL

#endif //GL_UNIFORM_WRAP_GLM

int main()
{
	auto a = GL::UniformWrap(std::vector< glm::mat4x3 >());

	static_assert(a, "asd");
}

#endif //TYPE_TRAITS_HPP
