#pragma once

#include "Framework/ATest.hpp"
#include "WorldObjects/Primitives/Plane.hpp"
#include "WorldObjects/Primitives/Triangle.hpp"
#include "WorldObjects/Complex/Cube.hpp"

namespace CG {
	namespace Test {
		class TestShapeSpawner : public ATest
		{
		public:
			TestShapeSpawner();
			~TestShapeSpawner();

			void onStart() override;
			void onUpdate(float deltaTime) override;
			void onRender() override;
			void onStop() override;
			void onReset() override;

		private:
			float _fov;

			std::unique_ptr<ShaderLoader> _sloader;
			std::vector<std::unique_ptr<AShape>> _shapes;
		};
	}
}