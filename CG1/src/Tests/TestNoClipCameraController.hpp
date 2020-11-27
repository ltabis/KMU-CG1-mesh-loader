#pragma once

#include "Framework/ATest.hpp"
#include "WorldObjects/Complex/Cube.hpp"
#include "NoClipCameraController.hpp"

namespace CG {
	namespace Test {
		class TestNoClipCameraController : public ATest
		{
		public:
			TestNoClipCameraController();
			~TestNoClipCameraController();

			void onStart() override;
			void onUpdate(float deltaTime) override;
			void onRender() override;
			void onStop() override;
			void onReset() override;

		private:
			std::unique_ptr<ShaderLoader> _sloader;
			std::unique_ptr<AShape> _cube;

			std::unique_ptr<NoClipCameraController> _controller;

			float _fov;
			bool _helpOpened;
			bool _fpsMode;
			glm::vec2 _aspectRatio;
		};
	}
}