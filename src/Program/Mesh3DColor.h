#ifndef MESH_3D_COLOR_H
#define MESH_3D_COLOR_H

#include "../Helper/Program.hpp"
#include "../Model/FrameParams.h"
#include "../Model/Material/MaterialParams.h"
#include "../Model/LightParams.h"
#include "../Model/Mesh/Mesh3D.hpp"

class Mesh3DColor
{
public:
	void Init();

	Mesh3DColor();

	void SetView(const glm::mat4 v)
	{
		frame.SetView(v);
	}

	void SetProj(const glm::mat4 p)
	{
		frame.SetProj(p);
	}

	const glm::mat4& GetView() const
	{
		return frame.GetView();
	}

	const glm::mat4& GetProj() const
	{
		return frame.GetProj();
	}

	Light* Lights()
	{
		return light.Lights();
	}

	void Update()
	{
		frame.Update();
		light.Update();
	}

	MaterialParams& GetMaterialParams()
	{
		return material;
	}

	void UseMaterial(size_t id)
	{
		material.UseMaterial(id);
	}

	void Use()
	{
		program.Use();
	}

	void Unuse()
	{
		program.Unuse();
	}

	void Render(Mesh3D& mesh)
	{
		mesh.Draw();
	};

	GL::Program& Program()
	{
		return program;
	}

private:
	static constexpr const char* vertFileName = "../shaders/blinPhongVert.glsl";
	static constexpr const char* fragFileName = "../shaders/blinPhongFrag.glsl";

	FrameParams frame;
	MaterialParams material;
	LightParams light;
	GL::Program program;
};

#endif //MESH_3D_COLOR_H
