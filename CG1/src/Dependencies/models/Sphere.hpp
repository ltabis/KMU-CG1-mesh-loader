#pragma once

#include "Components/ShaderLoader.hpp"
#include "WorldObjects/AShape.hpp"

class Sphere : public CG::AShape
{
public:
	Sphere();
	Sphere(float rad, GLuint sl, GLuint st);
	~Sphere();

private:
	float* m_V;
	float* m_N;
	float* m_Tex;
	unsigned int* m_El;
	
	GLuint nVerts, elements;
	float radius;
	GLuint slices, stacks;

	std::unique_ptr<CG::VertexBuffer> m_VBOVerts;
	std::unique_ptr<CG::VertexBuffer> m_VBONormals;
	std::unique_ptr<CG::IndexBuffer> m_IBO;
	std::unique_ptr<CG::VertexArrayLayout> m_VAL;
	std::unique_ptr<CG::VertexArray> m_VAO;

	void generateVerts(float* verts, float* norms, float* tex, unsigned int* el);

	const CG::VertexArray& vao() const override;
	const CG::IndexBuffer& ibo() const override;
};
