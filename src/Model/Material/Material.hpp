#ifndef MATERIAL_HPP

#include <GL/glew.h>

class Material
{
private:
	GLuint offset;
	GLuint size;
public:
	Material(GLuint _offset, GLuint _size) :
		offset(_offset),
		size(_size)
	{
	}

	GLuint GetOffset()
	{
		return offset;
	}

	GLuint GetSize()
	{
		return size;
	}
};

#endif //MATERIAL_HPP
