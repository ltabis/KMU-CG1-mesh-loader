#include "IndexBuffer.hpp"

CG::IndexBuffer::IndexBuffer(const unsigned int* data, unsigned int indices)
    : _indices { indices }
{
    glGenBuffers(1, &_id);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _id);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices * sizeof(unsigned int), data, GL_STATIC_DRAW);
}

CG::IndexBuffer::IndexBuffer(const std::vector<unsigned int>& indices)
    : _indices { indices.size() }
{
    glGenBuffers(1, &_id);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _id);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
}

CG::IndexBuffer::~IndexBuffer()
{
    glDeleteBuffers(1, &_id);
}

void CG::IndexBuffer::bind() const
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _id);
}

void CG::IndexBuffer::unbind() const
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

unsigned int CG::IndexBuffer::indices() const
{
    return _indices;
}
