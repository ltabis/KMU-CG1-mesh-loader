#pragma once

#include <vector>
#include <GL/glew.h>
#include "VertexBuffer.hpp"

namespace CG
{
	struct VertexBufferElement
	{
		unsigned int type;
		unsigned int count;
		unsigned char normalized;
	};

	class VertexArrayLayout
	{
	public:

		VertexArrayLayout()
			: _stride { 0 }
		{}

		template<typename T>
		void push(unsigned int count)
		{
			(void)count;
			throw "Cannot use this type to create a layout.";
		}

		template<>
		void push<float>(unsigned int count)
		{
			_layout.push_back(VertexBufferElement{ GL_FLOAT, count, GL_FALSE });
			_stride += count * sizeof(GL_FLOAT);
		}

		template<>
		void push<unsigned int>(unsigned int count)
		{
			_layout.push_back(VertexBufferElement{ GL_UNSIGNED_INT, count, GL_FALSE });
			_stride += count * sizeof(GL_UNSIGNED_INT);
		}

		template<>
		void push<unsigned char>(unsigned int count)
		{
			_layout.push_back(VertexBufferElement{ GL_UNSIGNED_BYTE, count, GL_FALSE });
			_stride += count * sizeof(GL_UNSIGNED_BYTE);
		}

		inline const std::vector<VertexBufferElement>& layout() const { return _layout; };
		inline unsigned int stride() const { return _stride; };

	private:
		std::vector<VertexBufferElement> _layout;
		size_t _stride;
	};
}
