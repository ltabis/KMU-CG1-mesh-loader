#include "vbotorus.hpp"


#include <GL/gl.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/constants.hpp>



#include <cstdio>
#include <cmath>

VBOTorus::VBOTorus(float outerRadius, float innerRadius, int nsides, int nrings) :
        rings(nrings), sides(nsides)
{
    faces = sides * rings;
    int nVerts  = sides * (rings+1);   // One extra ring to duplicate first ring

    // Verts
	m_V = new GLfloat[3 * nVerts];
    // Normals
	m_N = new GLfloat[3 * nVerts];

    // Elements
    m_El = new unsigned int[6 * faces];

    // Generate the vertex data
    generateVerts(m_V, m_N, m_El, outerRadius, innerRadius);

    //create vao, vbos, ibo here
    m_VBOVerts = std::make_unique<CG::VertexBuffer>(m_V, sizeof(GLfloat) * 3 * nVerts);
    m_VBONormals = std::make_unique<CG::VertexBuffer>(m_N, sizeof(GLfloat) * 3 * nVerts);

    // creating the layout of the buffer's data.
    m_VAL = std::make_unique<CG::VertexArrayLayout>();
    m_VAL->push<GLfloat>(3);

    // creating the vertex array, add the layout to it.
    m_VAO = std::make_unique<CG::VertexArray>();
    m_VAO->addBuffer(*m_VBOVerts, *m_VAL);
    m_VAO->addBuffer(*m_VBONormals, *m_VAL);

    // creating the index buffer.
    m_IBO = std::make_unique<CG::IndexBuffer>(m_El, faces * 6);

    glBindVertexArray(0);
}


VBOTorus::~VBOTorus()
{

}
void VBOTorus::generateVerts(GLfloat * verts, GLfloat * norms, unsigned int * el,
                             float outerRadius, float innerRadius)
{
	float TWOPI = 2 * glm::pi<float>();

    float ringFactor  = (float)(TWOPI / rings);
    float sideFactor = (float)(TWOPI / sides);
    int idx = 0, tidx = 0;
    for( int ring = 0; ring <= rings; ring++ ) {
        float u = ring * ringFactor;
        float cu = cos(u);
        float su = sin(u);
        for( int side = 0; side < sides; side++ ) {
            float v = side * sideFactor;
            float cv = cos(v);
            float sv = sin(v);
            float r = (outerRadius + innerRadius * cv);
            verts[idx] = r * cu;
            verts[idx + 1] = r * su;
            verts[idx + 2] = innerRadius * sv;
		
            norms[idx] = cv * cu * r;
            norms[idx + 1] = cv * su * r;
            norms[idx + 2] = sv * r;

//             tex[tidx] = (float)(u / TWOPI);
//             tex[tidx+1] = (float)(v / TWOPI);
//            tidx += 2;
            // Normalize
            float len = sqrt( norms[idx] * norms[idx] +
                              norms[idx+1] * norms[idx+1] +
                              norms[idx+2] * norms[idx+2] );
            norms[idx] /= len;
            norms[idx+1] /= len;
            norms[idx+2] /= len;
            idx += 3;
        }
    }

    idx = 0;
    for( int ring = 0; ring < rings; ring++ ) {
        int ringStart = ring * sides;
        int nextRingStart = (ring + 1) * sides;
        for( int side = 0; side < sides; side++ ) {
            int nextSide = (side+1) % sides;
            // The quad
            el[idx] = (ringStart + side);
            el[idx+1] = (nextRingStart + side);
            el[idx+2] = (nextRingStart + nextSide);
            el[idx+3] = ringStart + side;
            el[idx+4] = nextRingStart + nextSide;
            el[idx+5] = (ringStart + nextSide);
            idx += 6;
        }
    }


}

const CG::VertexArray& VBOTorus::vao() const
{
    return *m_VAO;
}

const CG::IndexBuffer& VBOTorus::ibo() const
{
    return *m_IBO;
}