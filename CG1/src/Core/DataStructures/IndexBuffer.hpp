#pragma once

#include <GL/glew.h>
#include <vector>

namespace CG
{
class IndexBuffer
{
public:
	IndexBuffer(const unsigned int* data, unsigned int indices);
	IndexBuffer(const std::vector<unsigned int>& data);
	~IndexBuffer();

	void bind() const;
	void unbind() const;
	unsigned int indices() const;
private:
	unsigned int _id;
	unsigned int _indices;
};
}