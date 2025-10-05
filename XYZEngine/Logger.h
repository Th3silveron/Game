#pragma once

#include "pch.h"
#include <fstream>
#include <string>
#include <mutex>
#include <chrono>
#include <sstream>
#include <cassert>

namespace XYZEngine
{
	enum class LogLevel
	{
		TRACE = 0,
		DEBUG = 1,
		INFO = 2,
		WARNING = 3,
		ERROR = 4,
		FATAL = 5
	};

	class Logger
	{
	public:
		static Logger& Instance();
		
		void Initialize(const std::string& logFilePath = "game.log");
		void Shutdown();
		
		void Log(LogLevel level, const std::string& message, const std::string& category = "GENERAL");
		
		// Convenience methods
		void Trace(const std::string& message, const std::string& category = "GENERAL");
		void Debug(const std::string& message, const std::string& category = "GENERAL");
		void Info(const std::string& message, const std::string& category = "GENERAL");
		void Warning(const std::string& message, const std::string& category = "GENERAL");
		void Error(const std::string& message, const std::string& category = "GENERAL");
		void Fatal(const std::string& message, const std::string& category = "GENERAL");
		
		void SetLogLevel(LogLevel level);
		void SetConsoleOutput(bool enabled);
		void SetFileOutput(bool enabled);

	private:
		Logger() = default;
		~Logger() = default;
		Logger(const Logger&) = delete;
		Logger& operator=(const Logger&) = delete;
		
		std::string GetTimestamp();
		std::string LogLevelToString(LogLevel level);
		
		LogLevel minLogLevel = LogLevel::TRACE;
		bool consoleOutput = true;
		bool fileOutput = true;
		std::ofstream logFile;
		std::mutex logMutex;
		bool initialized = false;
	};

	// Macro for easy logging
#define LOG_TRACE(msg, category) Logger::Instance().Trace(msg, category)
#define LOG_DEBUG(msg, category) Logger::Instance().Debug(msg, category)
#define LOG_INFO(msg, category) Logger::Instance().Info(msg, category)
#define LOG_WARNING(msg, category) Logger::Instance().Warning(msg, category)
#define LOG_ERROR(msg, category) Logger::Instance().Error(msg, category)
#define LOG_FATAL(msg, category) Logger::Instance().Fatal(msg, category)

	// Macro for assertions with logging
#define ASSERT_LOG(condition, msg, category) \
	do { \
		if (!(condition)) { \
			LOG_ERROR("ASSERTION FAILED: " + std::string(msg), category); \
			assert(condition); \
		} \
	} while(0)

	// Macro for throwing exceptions with logging
#define THROW_WITH_LOG(exceptionType, msg, category) \
	do { \
		LOG_ERROR("THROWING EXCEPTION: " + std::string(msg), category); \
		throw exceptionType(msg); \
	} while(0)
}
