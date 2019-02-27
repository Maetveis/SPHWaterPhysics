#ifndef GPU_ALLOCATOR_HPP
#define GPU_ALLOCATOR_HPP

#include <GL/glew.h>
#include <set>

class GPUAllocator
{
private:
	GLuint max;

	struct Hole
	{
		GLuint offset;
		GLuint size;

		Hole(GLuint _offset, GLuint _size = 0) :
			offset(_offset),
			size(_size)
		{
		}

		bool operator==(const Hole& rhs)
		{
			return offset == rhs.offset && size == rhs.size;
		}

		bool operator!=(const Hole& rhs)
		{
			return offset != rhs.offset || size != rhs.size;
		}
	};

	struct ByOffset
	{
		inline bool operator()(const Hole& lhs, const Hole& rhs) const
		{
			return lhs.offset < rhs.offset;
		}
	};

	struct ByLength
	{
		inline bool operator()(const Hole& lhs, const Hole& rhs) const
		{
			return lhs.size < rhs.size;
		}
	};

	std::set<Hole, ByOffset> holesByOffset;
	std::set<Hole, ByLength> holesByLength;

	void AddHole(GLuint offset, GLuint size);
public:
	GPUAllocator(GLuint max);
	~GPUAllocator();

	bool Allocate(GLuint size, GLuint alignment, GLuint* value);
	void DeAllocate(GLuint offset, GLuint length);

	GLuint GetSize() const
	{
		return max;
	}
};

#endif
