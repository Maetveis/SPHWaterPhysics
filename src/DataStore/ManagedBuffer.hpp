#ifndef MANAGED_BUFFER_HPP
#define MANAGED_BUFFER_HPP

#include "GPUAllocator.hpp"
#include "../Helper/Buffer.hpp"

class ManagedBuffer
{
private:
	GPUAllocator allocator;
	GL::Buffer buffer;
public:
	ManagedBuffer(GLuint size, GLenum usage) :
		allocator(size)
	{
		buffer.BufferData(size, nullptr, usage);
	}

	GLuint Reserve(GLuint size, GLuint alignment)
	{
		GLuint offset = Allocate(size, alignment);
		buffer.BufferSubData(offset, size, nullptr);

		return offset;
	}

	GLuint Push(GLuint size, void const * data, GLuint alignment)
	{
		GLuint offset = Allocate(size, alignment);
		buffer.BufferSubData(offset, size, data);

		return offset;
	}

	template <typename T>
	GLuint Push(const T& data, GLuint alignment)
	{
		return Push(sizeof(data), &data, alignment);
	}

	template <typename T>
	GLuint Push(const std::vector<T>& data, GLuint alignment)
	{
		return Push(sizeof(data[0]) * data.size(), data.data(), alignment);
	}

	GLuint Allocate(GLuint size, GLuint alignment)
	{
		GLuint offset;
		allocator.Allocate(size, alignment, &offset);

		return offset;
	}

	void Free(GLuint offset, GLuint length)
	{
		allocator.DeAllocate(offset, length);
	}

	GL::Buffer& GetBuffer()
	{
		return buffer;
	}

	const GL::Buffer& GetBuffer() const
	{
		return buffer;
	}
};

#endif
