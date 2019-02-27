#ifndef MATERIAL_PARAMS_H
#define MATERIAL_PARAMS_H

#include "../../Helper/UniformBuffer.hpp"
#include "../../DataStore/ManagedBuffer.hpp"

#include "ColorFormat.hpp"
#include "Material.hpp"

namespace GL {
	class Program;
}

class MaterialParams
{
public:
	MaterialParams() :
		buffer(30000, GL_STATIC_DRAW)
	{
	}

	void Bind(const GL::Program& program);

	size_t Push(const ColorFormat& material);

	void UseMaterial(size_t id);
private:
	static constexpr const char* blockName = "MaterialParams";

	std::vector<Material> materials;

	GL::UniformBuffer bindingIndex;
	ManagedBuffer buffer;
};

#endif //MATERIAL_PARAMS_H
