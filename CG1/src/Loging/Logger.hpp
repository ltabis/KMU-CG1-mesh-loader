#pragma once

#include <sstream>
#include "spdlog/spdlog.h"
#include "spdlog/sinks/ostream_sink.h"
#include "spdlog/sinks/stdout_color_sinks.h"

namespace CG {
	class Logger
	{
	public:
		static void Init();

		inline static std::shared_ptr<spdlog::logger>& profiler() { return _profiler; }
		inline static std::shared_ptr<spdlog::logger>& logger() { return _logger; }
		inline static const std::string& buffer() { return _buffer; }

		inline static void clearConsoleLogs() { _buffer.clear(); }

		template<typename... Args>
		static void log_to_str(spdlog::level::level_enum level, const std::string& msg, const Args& ... args)
		{
			std::ostringstream oss;
			auto oss_sink = std::make_shared<spdlog::sinks::ostream_sink_mt>(oss);
			spdlog::logger oss_logger("console_logger", oss_sink);

			oss_logger.set_level(level);
			oss_logger.set_pattern("%^[%T] (%l)%$ %v");
			oss_logger.log(level, msg, args...);
			_buffer.append(oss.str());
		}

	private:
		static std::shared_ptr<spdlog::logger> _profiler;
		static std::shared_ptr<spdlog::logger> _logger;
		inline static std::string _buffer;
	};
};

#ifdef _DEBUG
	// secure the fmt var.
	// ## __VA_ARGS__ deletes the ',' if there is no args.
	#define CG_LOG_TRACE(fmt, ...)          CG::Logger::logger()->trace(fmt, ## __VA_ARGS__);
	#define CG_LOG_INFO(fmt, ...)           CG::Logger::logger()->info(fmt, ##__VA_ARGS__);
	#define CG_LOG_WARN(fmt, ...)           CG::Logger::logger()->warn(fmt, ##__VA_ARGS__);
	#define CG_LOG_ERROR(fmt, ...)          CG::Logger::logger()->error(fmt, ##__VA_ARGS__);
	#define CG_LOG_CRITICAL(fmt, ...)       CG::Logger::logger()->critical(fmt, ##__VA_ARGS__);

	#define CG_CONSOLE_TRACE(fmt, ...)      CG::Logger::log_to_str(spdlog::level::trace, fmt, __VA_ARGS__);
	#define CG_CONSOLE_INFO(fmt, ...)		CG::Logger::log_to_str(spdlog::level::info, fmt, __VA_ARGS__);
	#define CG_CONSOLE_WARN(fmt, ...)		CG::Logger::log_to_str(spdlog::level::warn, fmt, __VA_ARGS__);
	#define CG_CONSOLE_ERROR(fmt, ...)		CG::Logger::log_to_str(spdlog::level::err, fmt, __VA_ARGS__);
	#define CG_CONSOLE_CRITICAL(fmt, ...)	CG::Logger::log_to_str(spdlog::level::critical, fmt, __VA_ARGS__);

	#define CG_PROFILER_TRACE(fmt, ...)     CG::Logger::profiler()->trace(fmt, ##__VA_ARGS__);
	#define CG_PROFILER_INFO(fmt, ...)      CG::Logger::profiler()->info(fmt, ##__VA_ARGS__);
	#define CG_PROFILER_WARN(fmt, ...)      CG::Logger::profiler()->warn(fmt, ##__VA_ARGS__);
	#define CG_PROFILER_ERROR(fmt, ...)     CG::Logger::profiler()->error(fmt, ##__VA_ARGS__);
	#define CG_PROFILER_CRITICAL(fmt, ...)  CG::Logger::profiler()->critical(fmt, ##__VA_ARGS__);
#endif /* !DEBUG */