#ifndef BUFFER_HPP
#define BUFFER_HPP

#include <GL/glew.h>
#include <vector>

#include "../Log/Logger.h"

namespace GL {

class Buffer
{
private:
	GLuint id;
public:
	Buffer()
	{
		glCreateBuffers(1, &id);
		Logger::Debug() << "Created glBuffer with id: " << id << '\n';
	}

	GLint GetId() const
	{
		return id;
	}

	void Bind(GLenum target) const
	{
		glBindBuffer(target, id);
	}

	void BufferData(GLuint size, const void* data, GLenum usage)
	{
		glNamedBufferData(id, size, data, usage);
	}

	template<typename T>
	void BufferData(const T& data, GLenum usage)
	{
		BufferData(sizeof(T), reinterpret_cast<const void *>(&data), usage);
	}

	template<typename T>
	void BufferData(const std::vector<T>& vector, GLenum usage)
	{
		BufferData(vector.size() * sizeof(T), reinterpret_cast<const void *>(vector.data()), usage);
	}

	void BufferSubData(GLuint offset, GLsizeiptr size, const void* data)
	{
		glNamedBufferSubData(id, offset, size, data);
	}

	template<typename T>
	void BufferSubData(GLuint offset, const T& data)
	{
		BufferSubData(offset, sizeof(data), reinterpret_cast<void *>(&data));
	}

	template<typename T>
	void BufferSubData(GLuint offset, const std::vector<T>& vector, GLenum usage)
	{
		BufferSubData(offset, vector.size() * sizeof(T), reinterpret_cast<const void *>(vector.data()));
	}

	void InitEmpty(GLuint size, GLenum usage)
	{
		BufferData(size, nullptr, usage);
	}

	~Buffer()
	{
		glDeleteBuffers(1, &id);
	}

};

}// namespace GL

#endif
