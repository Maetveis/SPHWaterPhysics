#ifndef BASIC_VERTEX_DESCRIPTOR_HPP
#define BASIC_VERTEX_DESCRIPTOR_HPP

#include "../../Helper/VertexArray.hpp"
#include "../../Helper/VertexArrayBinding.hpp"
#include "../../Helper/Buffer.hpp"
#include "../../Helper/TypeEnum.hpp"

#include "BasicVertexFormat.h"

#include <cstddef>

class BasicVertexDescriptor : public GL::VertexArray
{
private:
	GL::VertexArrayBinding binding;
public:
	BasicVertexDescriptor()
	{
		//First attrribute
		EnableAttrib(0);
		FormatAttrib(0, 3, GL::TypeEnum<float>::value, false, offsetof(BasicVertexFormat, pos));
		binding.BindAttrib(*this, 0);

		//Set Second attribute
		EnableAttrib(1);
		FormatAttrib(1, 3, GL::TypeEnum<float>::value, false, offsetof(BasicVertexFormat, norm));
		binding.BindAttrib(*this, 1);

		//Set Third attribute
		EnableAttrib(2);
		FormatAttrib(2, 2, GL::TypeEnum<float>::value, false, offsetof(BasicVertexFormat, uv));
		binding.BindAttrib(*this, 2);
	}

	inline void AttachVertex(const GL::Buffer& buffer)
	{
		binding.BindBuffer(*this, buffer, 0, sizeof(BasicVertexFormat));
	}

	inline void Bind() const
	{
		VertexArray::Bind();
	}

	inline GLint Get() const
	{
		return id;
	}
};

#endif //BASIC_VERTEX_DESCRIPTOR_HPP
