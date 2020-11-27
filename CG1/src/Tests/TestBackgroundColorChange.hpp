#pragma once

#include "Framework/ATest.hpp"

namespace CG {
	namespace Test {
		class TestBackgroundColorChange : public ATest
		{
		public:
			TestBackgroundColorChange();
			~TestBackgroundColorChange();

			void onStart() override;
			void onUpdate(float deltaTime) override;
			void onRender() override;
			void onStop() override;
			void onReset() override;

		private:
			float _color[4];
		};
	}
}