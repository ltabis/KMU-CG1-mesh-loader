#pragma once

#include <GL/glew.h>

namespace CG
{
class VertexBuffer
{
public:
	VertexBuffer(const void *data, unsigned int size);
	~VertexBuffer();

	void bind() const;
	void unbind() const;
private:
	unsigned int _id;
};
}