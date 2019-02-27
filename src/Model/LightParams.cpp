#include "LightParams.h"
#include "../Helper/Program.hpp"

void LightParams::Update()
{
	buffer.BufferData(data, GL_STATIC_DRAW);
}

void LightParams::Bind(const GL::Program& program)
{
	bindingPoint.AttachToBlock(program, program.GetUniformBlockIndex(blockName));
	bindingPoint.AttachBuffer(buffer);
}
