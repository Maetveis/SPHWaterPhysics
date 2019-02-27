#ifndef RIGID_MODEL_HPP
#define RIGID_MODEL_HPP

#include "Mesh/Mesh3D.hpp"
#include <vector>

using MaterialId = unsigned int;

class RigidModel
{
public:
	std::vector<Mesh3D> meshes;
	std::vector<MaterialId> materials;

	RigidModel() = default;
	RigidModel(const RigidModel&) = delete;
	RigidModel(RigidModel&&) = default;

	RigidModel& operator=(const RigidModel&) = delete;
	RigidModel& operator=(RigidModel&&) = default;
/*
	void AddPart(MeshId mesh, MaterialId material);*/
};
#endif
