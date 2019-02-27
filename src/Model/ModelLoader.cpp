#include "ModelLoader.hpp"

#include "../Log/Logger.h"
#include "Material/MaterialParams.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <glm/gtc/random.hpp>

#include <limits>

bool ModelLoader::ImportFile(const std::string& filename, RigidModel& newModel)
{
	Assimp::Importer importer;

	const aiScene* scene = importer.ReadFile(
		filename.c_str(),
			aiProcess_Triangulate |
			aiProcess_FlipUVs |
			aiProcess_GenNormals |
			aiProcess_JoinIdenticalVertices
	);

	if(!scene)
	{
		Logger::Warning << "Error parsing file: " << filename << "\nAssimp error: " << importer.GetErrorString() << '\n';
		return false;
	}

	for(unsigned int i = 0; i < scene->mNumMeshes; ++i)
	{
		std::pair<GLuint, GLuint> vertex = InsertVertices(GetVertices(*scene->mMeshes[i]));
		newModel.materials.emplace_back(materialParams.Push(ColorFormat(glm::sphericalRand<float>(1.) + glm::vec3(.4, .4, .4), glm::sphericalRand<float>(1.), glm::sphericalRand(1.),  30)));

		if(scene->mMeshes[i]->mNumVertices < std::numeric_limits<GLubyte>::max())
			HandleIndices<GLubyte>(*scene->mMeshes[i], newModel, vertex);
		else if (scene->mMeshes[i]->mNumVertices < std::numeric_limits<GLushort>::max())
			HandleIndices<GLushort>(*scene->mMeshes[i], newModel, vertex);
		else
			HandleIndices<GLuint>(*scene->mMeshes[i], newModel, vertex);
	}

	Logger::Debug << "Successfully loaded " << filename << '\n';

	return true;
}

template <typename T>
void ModelLoader::HandleIndices(const aiMesh& mesh, RigidModel& model, const std::pair<GLuint, GLuint>& vertex)
{
	std::vector<T> vec = GetIndices<T>(mesh);
	GLuint indexOffset = InsertIndices(vec.size() * sizeof(T), vec.data(), sizeof(T));
	model.meshes.emplace_back(vertex.first, (GLint) vertex.second, (char*)(0) + indexOffset, vec.size(), GL::TypeEnum<T>::value);
}

GLuint ModelLoader::InsertIndices (GLuint size, const void* data, GLuint alignment)
{
	GLuint offset = indexBuffer.Push(size, data, alignment);

	return offset;
}

std::pair<GLuint, GLuint> ModelLoader::InsertVertices(const std::vector<BasicVertexFormat>& vertices)
{
	GLuint vertexSize = static_cast<GLuint> (vertices.size() * sizeof(BasicVertexFormat));
	GLuint offset = vertexBuffer.Push(vertices, sizeof(BasicVertexFormat));

	return std::make_pair(offset / sizeof(BasicVertexFormat), vertexSize);
}

template <typename T>
std::vector<T> ModelLoader::GetIndices(const aiMesh& mesh)
{
	std::vector<T> indices;
	indices.reserve(mesh.mNumFaces * 3);

	for(unsigned face = 0; face < mesh.mNumFaces; ++face)
	{
		indices.emplace_back(mesh.mFaces[face].mIndices[0]);
		indices.emplace_back(mesh.mFaces[face].mIndices[1]);
		indices.emplace_back(mesh.mFaces[face].mIndices[2]);
	}
	return indices;
}

std::vector<BasicVertexFormat> ModelLoader::GetVertices(const aiMesh& mesh)
{
	std::vector<BasicVertexFormat> vertices;
	vertices.reserve(mesh.mNumVertices);

	const glm::vec2 zero = glm::vec2(0,0);
	for(unsigned i = 0; i < mesh.mNumVertices; ++i)
	{
		glm::vec3 const* p = reinterpret_cast<glm::vec3*>(&mesh.mVertices[i]);
		glm::vec3 const* n = reinterpret_cast<glm::vec3*>(&mesh.mNormals[i]);
		glm::vec2 const* uv = mesh.HasTextureCoords(i) ? reinterpret_cast<glm::vec2 const*>(&mesh.mTextureCoords[i]) : &zero;
		vertices.emplace_back(*p, *n, *uv);
	}

	return vertices;
}
