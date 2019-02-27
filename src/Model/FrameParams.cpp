#include "FrameParams.h"

#include "../Helper/Program.hpp"
#include "../Log/Logger.h"

void FrameParams::Bind(const GL::Program& program)
{
	Logger::Debug << "Fp " << blockName << " " << program.GetUniformBlockIndex(blockName) << '\n';
	bindingPoint.AttachToBlock(program, program.GetUniformBlockIndex(blockName));
	bindingPoint.AttachBuffer(buffer);
}

void FrameParams::Update()
{
	buffer.BufferData(data, GL_DYNAMIC_DRAW);
}
