#pragma once
#include <graphics/Texture.hpp>
#include <graphics/Buffer.hpp>
#include <graphics/Shader.hpp>

#include <glm/glm.hpp>
#include <memory>

struct MeshVertex {
	glm::vec3 a_Pos;
	glm::vec2 a_TexCoords;
	glm::vec3 a_Normal;
	glm::vec3 a_Tangent = glm::vec3(.0f);
	glm::vec3 a_Bitangent = glm::vec3(.0f);
	glm::vec4 a_Weights = glm::vec4(.0f);
	glm::ivec4 a_BoneIndices = glm::ivec4(-1);
};

struct Material
{
	std::shared_ptr<Texture2D> diffuse, specular, normal;
};

class Mesh 
{
public:
	Mesh(const Mesh&);
	Mesh(const std::vector<MeshVertex>& vertices, const std::vector<uint32_t>& indices, const Material& material);	

	void Render(Shader& shader);
private:
	std::unique_ptr<VertexArray> m_VAO;
	Material m_Material;
};