#pragma once
#include <graphics/Buffer.hpp>

#include <memory>

class glVertexBuffer : public VertexBuffer
{
public:
	~glVertexBuffer();
	glVertexBuffer() = delete;
	glVertexBuffer(const void* vertices, int32_t size);

	virtual void Bind() const override;
	virtual void Unbind() const override;

	virtual void SetLayout(const BufferLayout& layout) override { m_Layout = layout; }
	virtual BufferLayout GetLayout() const override { return m_Layout; }
private:
	BufferLayout m_Layout;
	uint32_t m_ID;
};

class glIndexBuffer : public IndexBuffer
{
public:
	~glIndexBuffer();
	glIndexBuffer(uint32_t count);
	glIndexBuffer(const std::vector<uint32_t>&);

	virtual void Bind() const override;
	virtual void Unbind() const override;

	virtual void SetData(const std::vector<uint32_t>& indices, uint32_t offset) override;

	virtual uint32_t GetCount() const override { return m_iCount; }
private:	
	uint32_t m_ID;
	/*indices count*/
	uint32_t m_iCount;
};


class glVertexArray : public VertexArray
{
public:
	~glVertexArray();
	glVertexArray();

	virtual void SetIndexBuffer(IndexBuffer*) override;
	virtual void AddVertexBuffer(VertexBuffer*) override;

	virtual const IndexBuffer* GetIndexBuffer() const override { return m_IndexBuffer.get(); }
	virtual VertexBuffer* GetVertexBuffer(int32_t index) const override { return m_VertexBuffers.size() > index ? m_VertexBuffers[index].get() : 0; }
	virtual int GetVertexBufferCount() const override { return m_VertexBuffers.size(); }

	virtual void Bind() const override;
	virtual void Unbind() const override;
private:
	std::unique_ptr<IndexBuffer> m_IndexBuffer;
	std::vector<std::unique_ptr<VertexBuffer>> m_VertexBuffers;
	int32_t m_VertexBufferIndex;
	uint32_t m_ID;
};

class glUniformBuffer : public UniformBuffer
{
public:
	glUniformBuffer(uint32_t size, uint32_t binding_point);
	~glUniformBuffer();

	virtual void Bind() override;
	virtual void SetData(const void* data, uint32_t offset, uint32_t size) override;
private:
	uint32_t m_ID, m_BindingPoint;
};


class glShaderStorageBuffer : public ShaderStorageBuffer
{
public:
	~glShaderStorageBuffer();
	glShaderStorageBuffer(uint32_t size, uint32_t binding_point);
	glShaderStorageBuffer(const void* data, uint32_t size, uint32_t binding_point);

	virtual	void Bind() override;
	virtual void SetData(const void* data, uint32_t offset, uint32_t size) override;
private:
	uint32_t m_ID, m_BindingPoint;
};