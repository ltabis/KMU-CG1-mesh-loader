// ShaderLoader.hpp
// shader loader class implementation.

#include "ShaderLoader.hpp"

CG::ShaderLoader::ShaderLoader()
	: _program(glCreateProgram())
{}

CG::ShaderLoader::~ShaderLoader()
{
	glDeleteProgram(_program);
}

/* parses the header line of a shader and create a shader. */
CG::Shader CG::ShaderLoader::findShader(std::string& line)
{
	for (auto& it : _shaderTypes) {
		std::size_t typeIndex = line.find(it.first);
		if (typeIndex != std::string::npos) {
			line.erase(std::remove_if(line.begin(), line.end(), isspace), line.end());
			return Shader{ it.second, line.substr(typeIndex + it.first.length() - 1, line.size()) };
		}
	}

	return Shader();
}

/* fetch the shader source code and returns it. */
std::string CG::ShaderLoader::getShaderSourceCode(std::ifstream& stream, std::string& line)
{
	std::ostringstream sourceCode;

	while (std::getline(stream, line)
		&& line.find("#shader") == std::string::npos)
		sourceCode << line << "\n";
	return sourceCode.str();
}

/* create a shader and adds it to the shader pool. */
void CG::ShaderLoader::createShader(std::ifstream& stream, std::string& line)
{
	// creating the shader.
	Shader shader = findShader(line);

	if (shader.type == ShaderType::NONE)
		return;

	// adding the source code.
	shader.source = getShaderSourceCode(stream, line);

	if (shader.compileShader())
		// adding the current shader to the map
		// if compilation is successful.
		_shaders.push_back(shader);
}

/* public api, load a shader by its name and file. */
bool CG::ShaderLoader::load(const std::string& name, const std::string& file)
{
	std::ifstream stream(file);

	if (!stream.is_open())
		return false;

	std::string line;
	std::istream& output = std::getline(stream, line);

	while (output)
		if (line.find("#shader") != std::string::npos
			&& line.find(name) != std::string::npos) {
			createShader(stream, line);
			return true;
		}
		else
			std::getline(stream, line);
	return false;
}

/* public api, load all shaders from a file. */
bool CG::ShaderLoader::load(const std::string& file)
{
	std::ifstream stream(file);

	if (!stream.is_open())
		return false;

	std::string line;
	std::istream& output = std::getline(stream, line);

	while (output)
		if (line.find("#shader") == std::string::npos)
			std::getline(stream, line);
		else
			createShader(stream, line);
	return true;
}

/* unload a shader from the shader pool. */
void CG::ShaderLoader::unload(const std::string& name)
{
	for (auto it = _shaders.begin(); it != _shaders.end(); ++it)
		if ((*it).name == name) {
			_shaders.erase(it);
			return;
		}
}

void CG::ShaderLoader::attach(const std::string& name)
{
	for (auto& shader : _shaders)
		if (shader.name == name && shader.id)
			glAttachShader(_program, shader.id);
}

void CG::ShaderLoader::attach()
{
	for (auto& shader : _shaders)
		if (shader.id)
			glAttachShader(_program, shader.id);
}

void CG::ShaderLoader::createExecutable()
{
	glLinkProgram(_program);
	glValidateProgram(_program);

	// deleting the intermidiate shaders since
	// they've been correctly loaded into the program.
	for (auto& shader : _shaders)
		glDeleteShader(shader.id);
}

int CG::ShaderLoader::findUniform(const std::string& uniformName)
{
	int location = 0;

	// The uniform location hasn't been retreived yet.
	if (_uniforms.find(uniformName) == _uniforms.end()) {
		location = glGetUniformLocation(_program, uniformName.c_str());
		if (location == -1) {
			CG_LOG_WARN("The uniform '{}' could not be found.", uniformName);
			return -1;
		}
		_uniforms.emplace(uniformName, location);
	}

	return location = !location ? _uniforms[uniformName] : location;
}

void CG::ShaderLoader::setUniform(const std::string& uniformName, const glm::mat4& matrix)
{
	use();
	int location = findUniform(uniformName);
	
	if (location == -1)
		return;
	glUniformMatrix4fv(location, 1, GL_FALSE, &matrix[0][0]);
}

void CG::ShaderLoader::use() const
{
	glUseProgram(_program);
}

/* get a shader object from its name. */
CG::Shader& CG::ShaderLoader::get(const std::string& name)
{
	for (auto& it : _shaders)
		if (it.name == name)
			return it;
	return _shaders[0];
}

/* creates and compiles the  */
bool CG::Shader::compileShader()
{
	const char* rawSource = source.c_str();

	glShaderSource(id, 1, &rawSource, nullptr);
	glCompileShader(id);

	// error handling.
	// wrapp this code into an error object with a logger.
	int status = 0;
	glGetShaderiv(id, GL_COMPILE_STATUS, &status);

	if (!status) {
		int messageLength = 0;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &messageLength);

		// stack allocated string.
		char* message = (char*)_malloca(sizeof(char) * messageLength);

		glGetShaderInfoLog(id, messageLength, &messageLength, message);

		CG_LOG_ERROR("Shader compilation failed. Error: {}", message);

		glDeleteShader(id);
		id = 0;
		return false;
	}
	return true;
}