#pragma once

#include <GL/glew.h>
#include <string>

#include "Logger.hpp"
#include "stb_image.hpp"

namespace CG
{
	class Texture
	{
	public:
		Texture(const std::string &filePath, const std::string &type);
		~Texture();

		bool loadTexture(const std::string &filePath);

		void bind() const;
		void unbind() const;

		inline std::string type() const { return m_Type; };
		inline std::string path() const { return m_FilePath; };
		inline unsigned int slot() const { return m_Slot; };
		inline int width() const { return m_Width; };
		inline int height() const { return m_Height; };
		inline int bytePerPixel() const { return m_BytePerPixel; };
	private:

		unsigned int m_Id;
		unsigned int m_Slot;
		unsigned char* m_Buffer;

		std::string m_FilePath;
		std::string m_Type;
		int m_Width;
		int m_Height;
		int m_BytePerPixel;

		// TODO: use it to optimise and use multiple texture
		// types per draw calls.
		inline static unsigned int m_CurrentTextureImageUnit;
	};
}