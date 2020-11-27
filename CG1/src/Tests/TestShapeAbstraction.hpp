#pragma once

#include "Framework/ATest.hpp"
#include "DataStructures/MatrixStack.hpp"
#include "WorldObjects/Primitives/Plane.hpp"
#include "WorldObjects/Primitives/Triangle.hpp"

namespace CG {
	namespace Test {
		class TestShapeAbstraction : public ATest
		{
		public:
			TestShapeAbstraction();
			~TestShapeAbstraction();

			void onStart() override;
			void onUpdate(float deltaTime) override;
			void onRender() override;
			void onStop() override;
			void onReset() override;

		private:
			float _fov;

			std::unique_ptr<ShaderLoader> _sloader;
			std::unique_ptr<AShape> _plane;
			std::unique_ptr<AShape> _triangle;
		};
	}
}