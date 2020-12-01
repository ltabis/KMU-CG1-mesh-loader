#include "VertexBuffer.hpp"

CG::VertexBuffer::VertexBuffer(const void* data, unsigned int size)
{
    glGenBuffers(1, &_id);
    glBindBuffer(GL_ARRAY_BUFFER, _id);
    glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
}

CG::VertexBuffer::VertexBuffer(const std::vector<Vertex>& data)
{
    glGenBuffers(1, &_id);
    glBindBuffer(GL_ARRAY_BUFFER, _id);
    glBufferData(GL_ARRAY_BUFFER,  data.size() * sizeof(Vertex), &data[0], GL_STATIC_DRAW);
}

CG::VertexBuffer::~VertexBuffer()
{
    glDeleteBuffers(1, &_id);
}

void CG::VertexBuffer::bind() const
{
    glBindBuffer(GL_ARRAY_BUFFER, _id);
}

void CG::VertexBuffer::unbind() const
{
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}