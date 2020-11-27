#pragma once

#include "ATest.hpp"

namespace CG {
	namespace Test {
		class TestMenu
		{
		public:
			TestMenu(std::shared_ptr<Renderer> &renderer, std::shared_ptr <GUI> gui);
			~TestMenu();
			void onRender();

			template<typename T>
			void registerTest(const std::string& testName)
			{
				if (testName == "")
					return;

				if (_tests.find(testName) == _tests.end()) {
					_tests.emplace(testName, std::make_unique<T>());
					_tests[testName]->onInit(_renderer);
					_tests[testName]->onStart();
				}
			}
		private:

			float _deltaTime;
			float _lastFrame;

			std::unordered_map<std::string, std::unique_ptr<ATest>> _tests;
			std::string _currentTest;
			std::shared_ptr<Renderer> _renderer;
			std::shared_ptr<GUI> _gui;
		};
	}
}