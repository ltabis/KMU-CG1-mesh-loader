#include "VertexArray.hpp"

CG::VertexArray::VertexArray()
	: _offset       { 0 }
	, _elementIndex { 0 }
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

	// binding the current vertex array and the buffer.
	bind();
	vb.bind();

	for (unsigned int i = 0; i < elements.size(); ++i, ++_elementIndex) {
		// specifying that we're adding a new layout element to the array.
		glEnableVertexAttribArray(_elementIndex);
		glVertexAttribPointer(_elementIndex,
			elements[i].count,
			elements[i].type,
			elements[i].normalized,
			layout.stride(),
			(const void*)_offset);

		_offset += elements[i].count * sizeof(elements[i].type);
	}
}
