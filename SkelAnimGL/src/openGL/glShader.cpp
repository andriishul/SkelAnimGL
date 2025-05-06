#include "glShader.hpp"
#include <utils/Logger.hpp>

#include "glad/glad.h"
#include <fstream>

std::string ReadShader(const std::string& filename)
{
	std::fstream fs{filename};

	if (fs.fail() || !fs.good())
		LOG_ERROR("Shader {} not found.\n", filename);
	std::string text, line;
	while (std::getline(fs, line))
		text += line + "\n";
	return text;
}
void CheckProgramLinkStatus(uint32_t program)
{
	GLint success = 1;
	glGetProgramiv(program, GL_LINK_STATUS, &success);
	if (!success) 
	{
		GLint length = 0;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length);
		GLchar* logMsg = new char[length];

		glGetProgramInfoLog(program, length, nullptr, logMsg);
		std::string errorMsg(logMsg);
		delete[] logMsg;
		LOG_ERROR(errorMsg);
	}
}

void CheckShaderCompileStatus(const std::string& shader_name, uint32_t shader_id)
{
	GLint success = 1;
	glGetShaderiv(shader_id, GL_COMPILE_STATUS, &success);
	if (!success) 
	{
		GLint length = 0;
		glGetShaderiv(shader_id, GL_INFO_LOG_LENGTH, &length);
		GLchar* logMsg = new char[length];

		glGetShaderInfoLog(shader_id, length, nullptr, logMsg);
		std::string errorMsg = shader_name + " " + logMsg;
		delete[] logMsg;
		LOG_ERROR(errorMsg);
	}
}

glShader::glShader(const std::string& vertexFile, const std::string& fragmentFile) : 
	m_ProgramID(glCreateProgram())
{
	std::string vertSrc = ReadShader(vertexFile);
	std::string fragSrc = ReadShader(fragmentFile);
	Compile(vertSrc, fragSrc);
}

glShader::~glShader()
{
	if(glIsProgram(m_ProgramID))
		glDeleteProgram(m_ProgramID);
}
void glShader::Compile(const std::string& vs_code, const std::string& fs_code)
{
	const char* vsCode = vs_code.c_str(), *fsCode = fs_code.c_str();
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER),
		fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	glShaderSource(vertexShader, 1, &vsCode, nullptr);
	glShaderSource(fragmentShader, 1, &fsCode, nullptr);

	glCompileShader(vertexShader);
	CheckShaderCompileStatus("OPENGL_VERTEX_SHADER::ERROR", vertexShader);
	glCompileShader(fragmentShader);
	CheckShaderCompileStatus("OPENGL_FRAGMENT_SHADER::ERROR", fragmentShader);

	glAttachShader(m_ProgramID, vertexShader);
	glAttachShader(m_ProgramID, fragmentShader);

	glLinkProgram(m_ProgramID);
	CheckProgramLinkStatus(m_ProgramID);

	glDetachShader(m_ProgramID, vertexShader);
	glDetachShader(m_ProgramID, fragmentShader);
	
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}

void glShader::Use() const
{
	glUseProgram(m_ProgramID);
}

void glShader::SetInt(const std::string& name, int32_t val) const
{
	glUniform1i(glGetUniformLocation(m_ProgramID, name.c_str()), val);
}

void glShader::SetFloat(const std::string& name, float_t val) const
{
	glUniform1f(glGetUniformLocation(m_ProgramID, name.c_str()), val);
}

void glShader::SetMat3(const std::string& name, glm::mat3 const& val) const
{
	glUniformMatrix3fv(glGetUniformLocation(m_ProgramID, name.c_str()), 1, false, &val[0][0]);
}

void glShader::SetMat4(const std::string& name, glm::mat4 const& val) const
{
	glUniformMatrix4fv(glGetUniformLocation(m_ProgramID, name.c_str()), 1, false, &val[0][0]); 
}

void glShader::SetVec2(const std::string& name, glm::vec2 const& val) const
{
	glUniform2f(glGetUniformLocation(m_ProgramID, name.c_str()), val.x,val.y);
}

void glShader::SetVec3(const std::string& name, glm::vec3 const& val) const
{
	glUniform3f(glGetUniformLocation(m_ProgramID, name.c_str()), val.x, val.y, val.z);
}

void glShader::SetVec4(const std::string& name, glm::vec4 const& val) const
{
	glUniform4f(glGetUniformLocation(m_ProgramID, name.c_str()), val.x, val.y, val.z, val.w);
}


glComputeShader::~glComputeShader()
{
	if (glIsProgram(m_ProgramID))
		glDeleteProgram(m_ProgramID);
}

glComputeShader::glComputeShader(const std::string& file)
{
	const std::string code = ReadShader(file);
	Compile(code);
}

void glComputeShader::Use()
{
	glUseProgram(m_ProgramID);
}

void glComputeShader::SetFloat(const std::string& name, float_t val)
{
	glUniform1f(glGetUniformLocation(m_ProgramID, name.c_str()), val);
}

void glComputeShader::Compile(const std::string& code) 
{
	const char* source = code.c_str();
	unsigned shaderID = glCreateShader(GL_COMPUTE_SHADER);
	glShaderSource(shaderID, 1, &source, nullptr);
	glCompileShader(shaderID);
	CheckShaderCompileStatus("OPENGL_COMPUTE_SHADER::ERROR", shaderID);

	m_ProgramID = glCreateProgram();
	glAttachShader(m_ProgramID, shaderID);
	glLinkProgram(m_ProgramID);
	CheckProgramLinkStatus(m_ProgramID);
	glDetachShader(m_ProgramID, shaderID);

	glDeleteShader(shaderID);
}
void glComputeShader::Dispatch(int32_t x, int32_t y, int32_t z)
{
	glDispatchCompute(x, y, z);
	glMemoryBarrier(GL_ALL_BARRIER_BITS);
}