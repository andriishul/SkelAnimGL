#pragma once
#include <graphics/Shader.hpp>

// Reads the contents of the shader file specified by the given filename and returns it as a string
std::string ReadShader(const std::string& filename);
// Checks the link status of the specified program and throws an error if linking fails
void CheckProgramLinkStatus(uint32_t program);
// Checks the compile status of the specified shader and throws an error if compilation fails
void CheckShaderCompileStatus(const std::string& shader_name, uint32_t shader_id);

class glShader : public Shader 
{
private:
	void Compile(const std::string& vs_code, const std::string& fs_code);
public:
	glShader(const std::string& vertexFile, const std::string& fragmentFile);
	virtual ~glShader();

	virtual void Use() const override;

	virtual void SetInt(const std::string& name, int32_t val) const override;
	virtual void SetFloat(const std::string& name, float_t val) const override;

	virtual void SetMat3(const std::string& name, glm::mat3 const&) const override;
	virtual void SetMat4(const std::string& name, glm::mat4 const&) const override;

	virtual void SetVec2(const std::string& name, glm::vec2 const&) const override;
	virtual void SetVec3(const std::string& name, glm::vec3 const&) const override;
	virtual void SetVec4(const std::string& name, glm::vec4 const&) const override;
private:
	uint32_t m_ProgramID;
};

class glComputeShader : public ComputeShader
{
private:
	void Compile(const std::string& code);
public:
	virtual ~glComputeShader();
	glComputeShader(const std::string& file);

	virtual void Use() override;
	virtual void SetFloat(const std::string& name, float_t val) override;

	virtual void Dispatch(int32_t x, int32_t y, int32_t z) override;
private:
	uint32_t m_ProgramID;
};
