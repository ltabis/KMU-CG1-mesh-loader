#pragma once

#include "Renderer.hpp"

namespace CG {
	namespace Test {
		class ATest {
		public:
			ATest() = default;
			virtual ~ATest() = default;

			void onInit(std::shared_ptr<Renderer>& renderer)
			{
				_renderer = renderer;
			}

			virtual void onStart() = 0;
			virtual void onUpdate(float deltaTime) = 0;
			virtual void onRender() = 0;
			virtual void onStop() = 0;
			virtual void onReset() = 0;

		protected:
			std::shared_ptr<Renderer> _renderer;
		};
	}
}