#include "Mesh3D.hpp"

Mesh3D::Mesh3D(GLint _vertexOffset, GLuint _vertexSize, void* _indexPointer, GLuint _indexCount, GLenum _type) :
	vertexOffset(_vertexOffset),
	vertexSize(_vertexSize),
	indexPointer(_indexPointer),
	indexCount(_indexCount),
	type(_type)
{
}

void Mesh3D::Draw() const
{
	glDrawElementsBaseVertex
	(
		GL_TRIANGLES,
		indexCount,
		type,
		indexPointer,
		vertexOffset
	);
}

GLint 	Mesh3D::GetOffset() const
{
	return vertexOffset;
}

GLuint 	Mesh3D::GetSize() const
{
	return vertexSize;
}

void * 	Mesh3D::GetIndexPointer() const
{
	return indexPointer;
}

GLuint 	Mesh3D::GetCount() const
{
	return indexCount;
}
