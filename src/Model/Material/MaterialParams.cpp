#include "MaterialParams.h"
#include "../../Helper/Program.hpp"

void MaterialParams::Bind(const GL::Program& program)
{
	bindingIndex.AttachToBlock(program, program.GetUniformBlockIndex(blockName));
	bindingIndex.AttachBuffer(buffer.GetBuffer());
}

size_t MaterialParams::Push(const ColorFormat& material)
{
	GLuint offset = buffer.Push(material, sizeof(ColorFormat));

	materials.emplace_back(offset, sizeof(ColorFormat));

	return materials.size() - 1;
}

void MaterialParams::UseMaterial(size_t id)
{
	bindingIndex.AttachBufferRange(buffer.GetBuffer(), materials[id].GetOffset(), materials[id].GetSize());
}
