#include "Mesh3DColor.h"

Mesh3DColor::Mesh3DColor()
{
	program.CreateName();
	program.VsFsProgram(vertFileName, fragFileName);

	frame.Bind(program);
	material.Bind(program);
	light.Bind(program);
}
