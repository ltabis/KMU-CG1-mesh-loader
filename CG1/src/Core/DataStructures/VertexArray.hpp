#pragma once

#include "VertexBuffer.hpp"
#include "VertexArrayLayout.hpp"
#include "Logger.hpp"

namespace CG
{
	class VertexArray
	{
	public:
		VertexArray();
		~VertexArray();

		// this method is static because we don't
		// need any of the object's data
		// to unbind vertex arrays.
		inline static void unbind() { glBindVertexArray(0); };
		inline void bind() const { glBindVertexArray(_id); };

		void addBuffer(const VertexBuffer& vb, const VertexArrayLayout& layout);
	private:
		unsigned int _id;
	};
}