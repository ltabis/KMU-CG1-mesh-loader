// ShaderLoader.hpp
// a class that will load shaders from a file.

#pragma once

#include <unordered_map>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <string>

#include <gl/glew.h>
#include <glm/mat4x4.hpp>
#include <Logger.hpp>

namespace CG
{
	enum class ShaderType { NONE = -1, VERTEX = GL_VERTEX_SHADER, FRAGMENT = GL_FRAGMENT_SHADER };

	const std::string DEFAULT_LIGHT_STRUCT =
		"struct LightInfo {\n"
		"	vec3 AmbiantColor;\n"
		"	vec3 DiffuseColor;\n"
		"	vec3 SpecularColor;\n"
		"	vec4 Position;\n"
		"	vec3 Intensity;\n"
		"};\n";

	const std::string DEFAULT_LIGHT_UNIFORM = "uniform LightInfo u_lights";
	const std::string DEFAULT_LIGHT_NUMBER = "int numberOfLights = ";

	struct Shader
	{
		Shader(const ShaderType& type = ShaderType::NONE, const std::string& name = "")
			: type   { type                                                        }
			, name   { name                                                        }
			, id     { type == ShaderType::NONE ? 0 : glCreateShader((GLenum)type) }
			, source { ""                                                          }
		{}

		Shader(const ShaderType& type, const std::string& name, const std::string& source)
			: type   { type                                                        }
			, name   { name                                                        }
			, id     { type == ShaderType::NONE ? 0 : glCreateShader((GLenum)type) }
			, source { source                                                      }
		{}

		~Shader() {

		}

		bool compileShader();

		ShaderType type;
		unsigned int id;
		std::string name;
		std::string source;
	};

	class ShaderLoader
	{
	private:
		unsigned int _program;
		std::unordered_map<std::string, unsigned int> _uniforms;
		std::vector<Shader> _shaders
		{
			Shader(),
		};
		std::unordered_map<std::string, ShaderType> _shaderTypes
		{
			{ "VERTEX",   ShaderType::VERTEX },
			{ "FRAGMENT", ShaderType::FRAGMENT }
		};

		void createShader(std::ifstream& stream, std::string& line, unsigned int numberOfLights);
		void addLightsToShader(Shader &shader, unsigned int numberOfLights);
		std::string getShaderSourceCode(std::ifstream& stream, std::string& line);
		Shader findShader(std::string& line);
		int findUniform(const std::string& uniformName);

	public:
		ShaderLoader();
		~ShaderLoader();

		bool load(const std::string& name, const std::string& file, unsigned int numberOfLights = 1);
		bool load(const std::string& file, unsigned int numberOfLights = 1);
		void unload(const std::string& name);
		void attach(const std::string& name);
		void attach();
		void createExecutable();
		void setUniform(const std::string& uniformName, int value);
		void setUniform(const std::string& uniformName, float value);
		void setUniform(const std::string& uniformName, const glm::mat4& matrix);
		void setUniform(const std::string& uniformName, const glm::mat3& matrix);
		void setUniform(const std::string& uniformName, const glm::vec4& vector);
		void setUniform(const std::string& uniformName, const glm::vec3& vector);

		void use() const;

		Shader& get(const std::string& name);
	};
}