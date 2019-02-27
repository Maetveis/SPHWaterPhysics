#ifndef BASIC_VERTEX_FORMAT_H
#define BASIC_VERTEX_FORMAT_H

#include <glm/vec3.hpp>
#include <glm/vec2.hpp>

struct BasicVertexFormat
{
	glm::vec3 pos;
	glm::vec3 norm;
	glm::vec2 uv;

	BasicVertexFormat(const glm::vec3& p, const glm::vec3& n, const glm::vec2& UV = glm::vec2(0,0)) :
		pos(p),
		norm(n),
		uv(UV)
	{
	}

	BasicVertexFormat(glm::vec3&& p, glm::vec3&& n, glm::vec2&& UV = glm::vec2(0,0)) :
		pos(p),
		norm(n),
		uv(UV)
	{
	}

};

#endif //BASIC_0VERTEXT_FORMAT_H
