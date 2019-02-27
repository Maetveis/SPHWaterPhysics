#ifndef MESH3D_H
#define MESH3D_H

#include <GL/glew.h>

class Mesh3D
{
private:
	GLint vertexOffset;
	GLuint vertexSize; // size taken on vertex buffer for deallocation

	void* indexPointer;
	GLuint indexCount;

	GLenum type;

public:
	Mesh3D(GLint _vertexOffset, GLuint _vertexSize, void* _indexPointer, GLuint _indexCount, GLenum _type = GL_UNSIGNED_SHORT);

	Mesh3D(const Mesh3D&) = delete;
	Mesh3D(Mesh3D&&) = default;

	Mesh3D& operator=(const Mesh3D&) = delete;

	Mesh3D& operator=(Mesh3D&&) = default;

	void Draw() const;

	GLint GetOffset() const;
	GLuint GetSize() const;
	void * GetIndexPointer() const;
	GLuint GetCount() const;
};

#endif //MESH3D_H
