#ifndef VBOTORUS_H
#define VBOTORUS_H

#include "GL/glew.h"
#include <glm/mat4x4.hpp>

#include "WorldObjects/AShape.hpp"

class VBOTorus : public CG::AShape
{
private:
    unsigned int vaoHandle;
    int faces, rings, sides;
	
	GLuint vbo_cube_vertices, vbo_cube_normals, ibo_cube_elements;
    void generateVerts(float * , float * , unsigned int *,
                       float , float);

    GLfloat* m_V;
    GLfloat* m_N;
    float* m_Tex;
    unsigned int* m_El;

    std::unique_ptr<CG::VertexBuffer> m_VBOVerts;
    std::unique_ptr<CG::VertexBuffer> m_VBONormals;
    std::unique_ptr<CG::IndexBuffer> m_IBO;
    std::unique_ptr<CG::VertexArrayLayout> m_VAL;
    std::unique_ptr<CG::VertexArray> m_VAO;

public:
    VBOTorus(float, float, int, int);
	virtual ~VBOTorus();

    const CG::VertexArray& vao() const override;
    const CG::IndexBuffer& ibo() const override;
};

#endif // VBOTORUS_H
