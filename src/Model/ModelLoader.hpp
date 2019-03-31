#ifndef MODEL_LOADER_HPP
#define MODEL_LOADER_HPP

#include "../DataStore/ManagedBuffer.hpp"
#include "Mesh/BasicVertexFormat.h"
#include "RigidModel.hpp"

#include "../Helper/TypeEnum.hpp"

#include <vector>
#include <string>
#include <type_traits>
#include <cstdlib>
#include <ctime>

class aiMesh;
class Mesh3D;
class MaterialParams;

class ModelLoader
{
private:
	ManagedBuffer& vertexBuffer;
	ManagedBuffer& indexBuffer;
	MaterialParams& materialParams;

	template <typename T>
	void HandleIndices(const aiMesh& mesh, RigidModel& model, const std::pair<GLuint, GLuint>& vertex);

	std::pair<GLuint, GLuint> InsertVertices(const std::vector<BasicVertexFormat>& vertices);
	GLuint InsertIndices (GLuint size, const void* data, GLuint alignment);

	template <typename T>
	std::vector<T> GetIndices(const aiMesh& mesh);

	template <typename T>
	void PushIndex(Mesh3D& mesh, RigidModel& model);

	std::vector<BasicVertexFormat> GetVertices(const aiMesh& mesh);
public:
	ModelLoader(ManagedBuffer& _vertexBuffer, ManagedBuffer& _indexBuffer, MaterialParams& _materialParams) :
		vertexBuffer(_vertexBuffer),
		indexBuffer(_indexBuffer),
		materialParams(_materialParams)
	{
		srand(time(0));
	}

	bool ImportFile(const std::string& filename, RigidModel& model);
};

#endif
