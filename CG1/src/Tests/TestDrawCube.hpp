#pragma once

#include "Framework/ATest.hpp"
#include "DataStructures/MatrixStack.hpp"

namespace CG {
	namespace Test {
		class TestDrawCube : public ATest
		{
		public:
			TestDrawCube();
			~TestDrawCube();

			void onStart() override;
			void onUpdate(float deltaTime) override;
			void onRender() override;
			void onStop() override;
			void onReset() override;

		private:

			MatrixStack _matrixStack;

			float _cubeVertices[56] = {
				// front
			   -1.0, -1.0, 1.0, 1.0,  	1.0, 0.0, 0.0,
				1.0, -1.0, 1.0, 1.0,	0.0, 1.0, 0.0,
				1.0,  1.0, 1.0, 1.0,	0.0, 0.0, 1.0,
			   -1.0,  1.0, 1.0,	1.0,	1.0, 1.0, 1.0,
				// back
			   -1.0, -1.0, -1.0, 1.0, 	1.0, 0.0, 0.0,
				1.0, -1.0, -1.0, 1.0,	0.0, 1.0, 0.0,
				1.0,  1.0, -1.0, 1.0,	0.0, 0.0, 1.0,
			   -1.0,  1.0, -1.0, 1.0,	1.0, 1.0, 1.0
			};

			unsigned int _cubeIndices[36] = {
				0, 1, 2,		2, 3, 0,		1, 5, 6,
				6, 2, 1,		7, 6, 5,		5, 4, 7,
				4, 0, 3,		3, 7, 4,		4, 5, 1,
				1, 0, 4,		3, 2, 6,		6, 7, 3,
			};

			glm::vec3 _rotation;
			glm::vec3 _translation;
			glm::vec3 _scale;
			float _fov;

			std::unique_ptr<VertexBuffer> _vbo;
			std::unique_ptr<VertexArray> _vao;
			std::unique_ptr<IndexBuffer> _ibo;
			std::unique_ptr<VertexArrayLayout> _dataLayout;
			std::unique_ptr<ShaderLoader> _sloader;
		};
	}
}