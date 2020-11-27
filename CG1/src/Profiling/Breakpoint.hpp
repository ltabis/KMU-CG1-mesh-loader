// Breakpoint.hpp
// Breakpoint class definition.

#pragma once

#include <chrono>
#include <iostream>
#include "Logger.hpp"

/* define classes for breakpoints with different time units. */
#define DEFINE_BREAKPOINT(name, type, unit) 		                 \
		static constexpr const char unit ## _str[] = #unit ## "\0";  \
		using name = CG::Breakpoint<type, unit ## _str>

namespace CG
{
	template<typename TimeResolution, const char* TimeUnit>
	class Breakpoint
	{
	public:

		Breakpoint(const std::string& name)
			: _name     { name                                      }
			, _start    { std::chrono::high_resolution_clock::now() }
		{
			CG_PROFILER_CRITICAL("breakpoint '{}' starting.", _name);
		}

		~Breakpoint()
		{
			auto t = std::chrono::duration_cast<TimeResolution>(std::chrono::high_resolution_clock::now() - _start).count();

			CG_PROFILER_CRITICAL("breakpoint '{}' done. Time: {}{}.", _name, t, std::string(TimeUnit));
		}

	private:
		std::string _name;
		std::string _timeUnit;
		std::chrono::high_resolution_clock::time_point _start;
	};

	// creating classes.
	DEFINE_BREAKPOINT(BreakpointS, std::chrono::seconds, s);
	DEFINE_BREAKPOINT(BreakpointMs, std::chrono::milliseconds, ms);
	DEFINE_BREAKPOINT(BreakpointUs, std::chrono::microseconds, us);
}

#undef DEFINE_BREAKPOINT