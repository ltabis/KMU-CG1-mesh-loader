#pragma once

#include "Framework/ATest.hpp"

namespace CG {
	namespace Test {
		class TestDrawTriangle : public ATest
		{
		public:
			TestDrawTriangle();
			~TestDrawTriangle();

			void onStart() override;
			void onUpdate(float deltaTime) override;
			void onRender() override;
			void onStop() override;
			void onReset() override;

		private:
			std::unique_ptr<VertexBuffer> _vbo;
			std::unique_ptr<VertexArray> _vao;
			std::unique_ptr<IndexBuffer> _ibo;
			std::unique_ptr<ShaderLoader> _shaders;
			std::unique_ptr<VertexArrayLayout> _layout;

			float _data[28] = {
				-0.5f, -0.5f, 0.0f, 1.0f,    1.0f,  0.0f, 0.0f,
				 0.5f, -0.5f, 0.0f, 1.0f,    0.0f,  1.0f, 0.0f,
				 0.5f,  0.5f, 0.0f, 1.0f,    0.0f,  0.0f, 1.0f,
				-0.5f,  0.5f, 0.0f, 1.0f,    1.0f,  1.0f, 1.0f
			};

			unsigned int _indices[6] = {
				0, 1, 2,
				2, 3, 0
			};

		};
	}
}