#include "VertexArray.hpp"

CG::VertexArray::VertexArray()
{
	glCreateVertexArrays(1, &_id);
	glBindVertexArray(_id);
}

CG::VertexArray::~VertexArray()
{
	glDeleteVertexArrays(1, &_id);
}

void CG::VertexArray::addBuffer(const VertexBuffer& vb, const VertexArrayLayout& layout)
{
	const auto& elements = layout.layout();
	unsigned int offset = 0;

	// binding the current vertex array and the buffer.
	bind();
	vb.bind();

	for (unsigned int i = 0; i < elements.size(); ++i) {
		// specifying that we're adding a new layout element to the array.
		glEnableVertexAttribArray(i);
		glVertexAttribPointer(i,
			elements[i].count,
			elements[i].type,
			elements[i].normalized,
			layout.stride(),
			(const void*)offset);

		offset += elements[i].count * sizeof(elements[i].type);
	}
}
