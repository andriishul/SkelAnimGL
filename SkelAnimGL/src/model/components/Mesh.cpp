#include "Mesh.hpp"
#include <graphics/RendererContext.hpp>

Mesh::Mesh(const std::vector<MeshVertex>& vertices, const std::vector<uint32_t>& indices, const Material& material)
	:	m_VAO(std::unique_ptr<VertexArray>(VertexArray::Create())), m_Material(material)

{
    RendererContext::Init();
    VertexBuffer* vbo = VertexBuffer::Create(vertices.data(), vertices.size() * sizeof(MeshVertex));
    vbo->SetLayout({
        BufferElement("a_Pos", ShaderDataType::Float3),
        BufferElement("a_TexCoords", ShaderDataType::Float2),
        BufferElement("a_Normal", ShaderDataType::Float3),
        BufferElement("a_Tangent", ShaderDataType::Float3),
        BufferElement("a_Bitangent", ShaderDataType::Float3),
        BufferElement("a_Weights", ShaderDataType::Float4),
        BufferElement("a_BoneIndices", ShaderDataType::Int4)
    });
    m_VAO->AddVertexBuffer(vbo);
    m_VAO->SetIndexBuffer(IndexBuffer::Create(indices));
}

void Mesh::Render(Shader& shader)
{
    shader.Use();
    if(m_Material.diffuse)
        m_Material.diffuse->Bind(0);
    if (m_Material.specular)
        m_Material.specular->Bind(1);
    if (m_Material.normal)
        m_Material.normal->Bind(2);
    RendererContext::DrawIndexed(*m_VAO);
}
