#pragma once

#include "Framework/ATest.hpp"
#include "WorldObjects/Complex/Cube.hpp"
#include "Camera.hpp"

namespace CG {
	namespace Test {
		class TestCamera : public ATest
		{
		public:
			TestCamera();
			~TestCamera();

			void onStart() override;
			void onUpdate(float deltaTime) override;
			void onRender() override;
			void onStop() override;
			void onReset() override;

		private:
			std::unique_ptr<ShaderLoader> _sloader;
			std::unique_ptr<AShape> _cube;

			std::unique_ptr<Camera> _camera;

			float _fov;
			glm::vec2 _aspectRatio;
		};
	}
}