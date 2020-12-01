#pragma once

#include <GL/glew.h>
#include "DataStructures/Vertex.hpp"

#include <vector>

namespace CG
{
class VertexBuffer
{
public:
	VertexBuffer(const void* data, unsigned int size);
	VertexBuffer(const std::vector<Vertex>& data);
	~VertexBuffer();

	void bind() const;
	void unbind() const;
private:
	unsigned int _id;
};
}