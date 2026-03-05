#include "glBuffer.hpp"
#include "glad/glad.h"
#include "utils/Logger.hpp"

glVertexBuffer::~glVertexBuffer()
{
	if (glIsBuffer(m_ID))
		glDeleteBuffers(1, &m_ID);
}


glVertexBuffer::glVertexBuffer(const void* vertices, int32_t size)
{
	glGenBuffers(1, &m_ID);
	Bind();
	glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
	Unbind();
}

void glVertexBuffer::Bind() const
{
	glBindBuffer(GL_ARRAY_BUFFER, m_ID);
}

void glVertexBuffer::Unbind() const
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}
static GLenum ShaderDataTypeToOpenGLBaseType(ShaderDataType type)
{
	switch (type)
	{
	case ShaderDataType::Float:    return GL_FLOAT;
	case ShaderDataType::Float2:   return GL_FLOAT;
	case ShaderDataType::Float3:   return GL_FLOAT;
	case ShaderDataType::Float4:   return GL_FLOAT;
	case ShaderDataType::Mat3:     return GL_FLOAT;
	case ShaderDataType::Mat4:     return GL_FLOAT;
	case ShaderDataType::Int:      return GL_INT;
	case ShaderDataType::Int2:     return GL_INT;
	case ShaderDataType::Int3:     return GL_INT;
	case ShaderDataType::Int4:     return GL_INT;
	case ShaderDataType::Bool:     return GL_BOOL;
	}
	LOG_ERROR("Shader data type not found.");
	return 0;
}


glVertexArray::~glVertexArray()
{
	if (glIsVertexArray(m_ID))
		glDeleteVertexArrays(1, &m_ID);
}

glVertexArray::glVertexArray() : m_VertexBufferIndex(0)
{
	glGenVertexArrays(1, &m_ID);
}
void glVertexArray::SetIndexBuffer(IndexBuffer* ibo)
{
	Bind();
	ibo->Bind();
	m_IndexBuffer = std::unique_ptr<IndexBuffer>(ibo);
	Unbind();
}

static bool IsIntType(ShaderDataType type) 
{
	return type == ShaderDataType::Int || type == ShaderDataType::Int2 || type == ShaderDataType::Int3 || type == ShaderDataType::Int4;
}
void glVertexArray::AddVertexBuffer(VertexBuffer* vbo)
{
	Bind();
	
	vbo->Bind();
	auto layout = vbo->GetLayout();
	int stride = layout.GetStride();
	for (auto& element : layout)
	{
		if (element.shaderType == ShaderDataType::Mat3 || element.shaderType == ShaderDataType::Mat4)
		{
				int count = element.GetComponentCount();
				for (int i = 0; i < count; i++)
				{
					glEnableVertexAttribArray(m_VertexBufferIndex);
					glVertexAttribPointer(m_VertexBufferIndex,
						count,
						ShaderDataTypeToOpenGLBaseType(element.shaderType),
						element.normalized,
						stride,
						(void*)(element.offset + sizeof(float) * count * i));
					glVertexAttribDivisor(m_VertexBufferIndex, 1);
				}
		}
		else {
			glEnableVertexAttribArray(m_VertexBufferIndex);

			if(IsIntType(element.shaderType) || element.shaderType ==  ShaderDataType::Bool)
				glVertexAttribIPointer(m_VertexBufferIndex,
					element.GetComponentCount(),
					ShaderDataTypeToOpenGLBaseType(element.shaderType),
					stride,
					(void*)element.offset);
			else
			glVertexAttribPointer(m_VertexBufferIndex,
				element.GetComponentCount(),
				ShaderDataTypeToOpenGLBaseType(element.shaderType),
				element.normalized,
				stride,
				(void*)element.offset);
		}
		m_VertexBufferIndex++;
	}
	vbo->Unbind();
	Unbind();
	m_VertexBuffers.push_back(std::unique_ptr<VertexBuffer>(vbo));
}

void glVertexArray::Bind() const
{
	glBindVertexArray(m_ID);
}

void glVertexArray::Unbind() const
{
	glBindVertexArray(0);
}

glIndexBuffer::~glIndexBuffer()
{
	if (glIsBuffer(m_ID))
		glDeleteBuffers(1, &m_ID);
}

glIndexBuffer::glIndexBuffer(uint32_t count) : m_iCount(count)
{
	glGenBuffers(1, &m_ID);
	Bind();
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint32_t), nullptr, GL_DYNAMIC_DRAW);
	Unbind();
}

glIndexBuffer::glIndexBuffer(const std::vector<uint32_t>& indices):m_iCount(indices.size())
{
	glGenBuffers(1, &m_ID);
	Bind();
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(uint32_t), indices.data(), GL_STATIC_DRAW);
	Unbind();
}

void glIndexBuffer::Bind() const
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ID);
}

void glIndexBuffer::Unbind() const
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void glIndexBuffer::SetData(const std::vector<uint32_t>& indices, uint32_t offset)
{
	Bind();
	glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, offset, indices.size() * sizeof(uint32_t), indices.data());
	Unbind();
}

glUniformBuffer::glUniformBuffer(uint32_t size, uint32_t binding_point) : m_BindingPoint(binding_point)
{
	glGenBuffers(1, &m_ID);
	Bind();
	glBufferData(GL_UNIFORM_BUFFER, size, 0, GL_DYNAMIC_DRAW);

	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

glUniformBuffer::~glUniformBuffer()
{
	if (glIsBuffer(m_ID))
		glDeleteBuffers(1, &m_ID);
}


void glUniformBuffer::Bind()
{
	glBindBuffer(GL_UNIFORM_BUFFER, m_ID);
	glBindBufferBase(GL_UNIFORM_BUFFER, m_BindingPoint, m_ID);
}

void glUniformBuffer::SetData(const void* data, uint32_t offset, uint32_t size)
{
	Bind();
	glBufferSubData(GL_UNIFORM_BUFFER, offset, size, data);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

glShaderStorageBuffer::~glShaderStorageBuffer()
{
	if(glIsBuffer(m_ID))
		glDeleteBuffers(1, &m_ID);
}

glShaderStorageBuffer::glShaderStorageBuffer(uint32_t size, uint32_t binding_point) : m_BindingPoint(binding_point)
{
	glGenBuffers(1, &m_ID);
	Bind();
	glBufferData(GL_SHADER_STORAGE_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
}
glShaderStorageBuffer::glShaderStorageBuffer(const void* data, uint32_t size, uint32_t binding_point) : m_BindingPoint(binding_point)
{
	glGenBuffers(1, &m_ID);
	Bind();
	glBufferData(GL_SHADER_STORAGE_BUFFER, size, data, GL_STATIC_DRAW);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
}

void glShaderStorageBuffer::Bind()
{
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_ID);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, m_BindingPoint, m_ID);
}

void glShaderStorageBuffer::SetData(const void* data, uint32_t offset, uint32_t size)
{
	Bind();
	void* ptr = glMapBufferRange(GL_SHADER_STORAGE_BUFFER, offset, size,
		GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_RANGE_BIT | GL_MAP_UNSYNCHRONIZED_BIT);

	if (ptr) {
		memcpy(ptr, data, size);
		glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
	}
	else LOG_ERROR("Failed to upload data.");

}
