#include "Logger.hpp"

std::shared_ptr<spdlog::logger> CG::Logger::_profiler;
std::shared_ptr<spdlog::logger> CG::Logger::_logger;

/* initialize both logger and profiler. */
void CG::Logger::Init()
{
	_profiler = spdlog::stdout_color_mt("profiler");
	_profiler->set_pattern("%^[%T] (%n)%$ %v");
	_profiler->set_level(spdlog::level::trace);

	_logger = spdlog::stdout_color_mt("log");
	_logger->set_pattern("%^[%T] (%n)%$ %v");
	_logger->set_level(spdlog::level::trace);
}