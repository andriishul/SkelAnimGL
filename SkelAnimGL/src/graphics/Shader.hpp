#pragma once
#include <glm/glm.hpp>
#include <string>

#include <unordered_map>
#include <memory>

class Shader 
{
public:
	virtual ~Shader() = default;

	virtual void Use() const = 0;

	virtual void SetInt(const std::string& name, int32_t val) const = 0;
	virtual void SetFloat(const std::string& name, float_t val) const = 0;
	
	virtual void SetMat3(const std::string& name, glm::mat3 const&) const = 0;
	virtual void SetMat4(const std::string& name, glm::mat4 const&) const = 0;
	
	virtual void SetVec2(const std::string& name, glm::vec2 const&) const = 0;
	virtual void SetVec3(const std::string& name, glm::vec3 const&) const = 0;
	virtual void SetVec4(const std::string& name, glm::vec4 const&) const = 0;

	static std::shared_ptr<Shader> Get(const std::string& name);
private:
	static std::unordered_map<std::string, std::shared_ptr<Shader>> s_Shaders;
}; 

class ComputeShader 
{
public:
	virtual ~ComputeShader() = default;

	virtual void Use() = 0;
	virtual void SetFloat(const std::string& name, float_t val) = 0;
	virtual void Dispatch(int32_t x, int32_t y, int32_t z) = 0;
	
	static std::shared_ptr<ComputeShader> Get(const std::string& name);
private:
	static std::unordered_map<std::string, std::shared_ptr<ComputeShader>> s_computeShaders;
};