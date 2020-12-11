#include "Sphere.hpp"

Sphere::Sphere()
{

}

Sphere::~Sphere()
{
	delete[] m_V;
	delete[] m_N;
	delete[] m_Tex;
	delete[] m_El;
}

Sphere::Sphere(float rad, GLuint sl, GLuint st)
	: radius { rad }
	, slices { sl  }
	, stacks { st  }
{
	nVerts = (slices + 1) * (stacks + 1);
	elements = (slices * 2 * (stacks - 1)) * 3;

	// Verts
	m_V = new float[3 * nVerts];
	// Normals
	m_N = new float[3 * nVerts];  
	// Tex coords
	m_Tex = new float[2 * nVerts]; // we don't use it now
	// Index
	m_El = new unsigned int[elements];  //index

	// Generate the vertex data
	generateVerts(m_V, m_N, m_Tex, m_El);

	//create vao, vbo and ibo here... (We didn't use std::vector here...)
	m_VBOVerts = std::make_unique<CG::VertexBuffer>(m_V, sizeof(float) * 3 * nVerts);
	m_VBONormals = std::make_unique<CG::VertexBuffer>(m_N, sizeof(float) * 3 * nVerts);

	// creating the layout of the buffer's data.
	m_VAL = std::make_unique<CG::VertexArrayLayout>();
	m_VAL->push<float>(3);

	// creating the vertex array, add the layout to it.
	m_VAO = std::make_unique<CG::VertexArray>();
	m_VAO->addBuffer(*m_VBOVerts, *m_VAL);
	m_VAO->addBuffer(*m_VBONormals, *m_VAL);

	// creating the index buffer.
	m_IBO = std::make_unique<CG::IndexBuffer>(m_El, elements);
}

void Sphere::generateVerts(float* verts, float* norms, float* tex, unsigned int* el)
{
	// Generate positions and normals
	GLfloat theta, phi;
	GLfloat thetaFac = glm::two_pi<float>() / slices;
	GLfloat phiFac = glm::pi<float>() / stacks;
	GLfloat nx, ny, nz, s, t;
	GLuint idx = 0, tIdx = 0;
	for (GLuint i = 0; i <= slices; i++) {
		theta = i * thetaFac;
		s = (GLfloat)i / slices;
		for (GLuint j = 0; j <= stacks; j++) {
			phi = j * phiFac;
			t = (GLfloat)j / stacks;
			nx = sinf(phi) * cosf(theta);
			ny = sinf(phi) * sinf(theta);
			nz = cosf(phi);
			verts[idx] = radius * nx; verts[idx + 1] = radius * ny; verts[idx + 2] = radius * nz;
			norms[idx] = nx; norms[idx + 1] = ny; norms[idx + 2] = nz;
			idx += 3;

			tex[tIdx] = s;
			tex[tIdx + 1] = t;
			tIdx += 2;
		}
	}

	// Generate the element list
	idx = 0;
	for (GLuint i = 0; i < slices; i++) {
		GLuint stackStart = i * (stacks + 1);
		GLuint nextStackStart = (i + 1) * (stacks + 1);
		for (GLuint j = 0; j < stacks; j++) {
			if (j == 0) {
				el[idx] = stackStart;
				el[idx + 1] = stackStart + 1;
				el[idx + 2] = nextStackStart + 1;
				idx += 3;
			}
			else if (j == stacks - 1) {
				el[idx] = stackStart + j;
				el[idx + 1] = stackStart + j + 1;
				el[idx + 2] = nextStackStart + j;
				idx += 3;
			}
			else {
				el[idx] = stackStart + j;
				el[idx + 1] = stackStart + j + 1;
				el[idx + 2] = nextStackStart + j + 1;
				el[idx + 3] = nextStackStart + j;
				el[idx + 4] = stackStart + j;
				el[idx + 5] = nextStackStart + j + 1;
				idx += 6;
			}
		}
	}
}

const CG::VertexArray& Sphere::vao() const
{
	return *m_VAO;
}

const CG::IndexBuffer& Sphere::ibo() const
{
	return *m_IBO;
}

