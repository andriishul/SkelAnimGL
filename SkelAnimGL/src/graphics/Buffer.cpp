#include "Buffer.hpp"
#include <openGL/glBuffer.hpp>
VertexBuffer* VertexBuffer::Create(const void* vertices, uint32_t size)
{
    return new glVertexBuffer(vertices, size);
}

IndexBuffer* IndexBuffer::Create(const std::vector<uint32_t>& indices)
{
    return new glIndexBuffer(indices);
}

UniformBuffer* UniformBuffer::Create(uint32_t size, uint32_t binding_point)
{
    return new glUniformBuffer(size, binding_point);
}

VertexArray* VertexArray::Create()
{
    return new glVertexArray;
}

ShaderStorageBuffer* ShaderStorageBuffer::Create(uint32_t size, uint32_t binding_point)
{
    return new glShaderStorageBuffer(size, binding_point);
}

ShaderStorageBuffer* ShaderStorageBuffer::Create(const void* data, uint32_t size, uint32_t binding_point)
{
    return new glShaderStorageBuffer(data, size, binding_point);
}