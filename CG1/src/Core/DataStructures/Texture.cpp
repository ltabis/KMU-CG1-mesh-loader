#include "Texture.hpp"

CG::Texture::Texture(const std::string& filePath, const std::string& type)
	: m_Id			 { 0		}
	, m_Slot         { 0		}
	, m_Buffer		 { nullptr	}
	, m_FilePath	 { filePath	}
	, m_Type		 { type		}
	, m_Width		 { 0		}
	, m_Height		 { 0		}
	, m_BytePerPixel { 0		}
{
	// generate memory onto gpu.
	glGenTextures(1, &m_Id);
	glBindTexture(GL_TEXTURE_2D, m_Id);

	// specify default texture parameters.
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	// loading the texture.
	loadTexture(filePath);

	++m_CurrentTextureImageUnit;
}

CG::Texture::~Texture()
{
	glDeleteTextures(1, &m_Id);
}

bool CG::Texture::loadTexture(const std::string &filePath)
{
	// assigning a new file path.
	m_FilePath = filePath;

	// load bytes into memory.
	stbi_set_flip_vertically_on_load(1);
	m_Buffer = stbi_load(filePath.c_str(), &m_Width, &m_Height, &m_BytePerPixel, 4);

	if (!m_Buffer) {
		CG_LOG_ERROR("Couldn't load the '{}' texture: {}", m_FilePath, stbi_failure_reason());
		return false;
	}

	// sending the data to the gpu.
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_Buffer);
	unbind();

	// deleting cpu data since it's not needed anymore.
	stbi_image_free(m_Buffer);

	return true;
}

void CG::Texture::bind() const
{
	glActiveTexture(GL_TEXTURE0 + m_Slot);
	glBindTexture(GL_TEXTURE_2D, m_Id);
}

void CG::Texture::unbind() const
{
	glBindTexture(GL_TEXTURE_2D, 0);
}
