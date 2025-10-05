#include "pch.h"
#include "Logger.h"
#include <iostream>
#include <iomanip>

namespace XYZEngine
{
	Logger& Logger::Instance()
	{
		static Logger instance;
		return instance;
	}

	void Logger::Initialize(const std::string& logFilePath)
	{
		std::lock_guard<std::mutex> lock(logMutex);
		
		if (initialized)
		{
			Warning("Logger already initialized", "LOGGER");
			return;
		}

		if (fileOutput)
		{
			logFile.open(logFilePath, std::ios::out | std::ios::trunc);
			if (!logFile.is_open())
			{
				std::cerr << "Failed to open log file: " << logFilePath << std::endl;
				fileOutput = false;
			}
		}

		initialized = true;
		Info("Logger initialized successfully", "LOGGER");
	}

	void Logger::Shutdown()
	{
		std::lock_guard<std::mutex> lock(logMutex);
		
		if (initialized)
		{
			Info("Logger shutting down", "LOGGER");
			if (logFile.is_open())
			{
				logFile.close();
			}
			initialized = false;
		}
	}

	void Logger::Log(LogLevel level, const std::string& message, const std::string& category)
	{
		if (level < minLogLevel)
			return;

		std::lock_guard<std::mutex> lock(logMutex);
		
		if (!initialized)
			return;

		std::string timestamp = GetTimestamp();
		std::string levelStr = LogLevelToString(level);
		std::string logMessage = "[" + timestamp + "] [" + levelStr + "] [" + category + "] " + message;

		if (consoleOutput)
		{
			if (level >= LogLevel::ERROR)
			{
				std::cerr << logMessage << std::endl;
			}
			else
			{
				std::cout << logMessage << std::endl;
			}
		}

		if (fileOutput && logFile.is_open())
		{
			logFile << logMessage << std::endl;
			logFile.flush();
		}
	}

	void Logger::Trace(const std::string& message, const std::string& category)
	{
		Log(LogLevel::TRACE, message, category);
	}

	void Logger::Debug(const std::string& message, const std::string& category)
	{
		Log(LogLevel::DEBUG, message, category);
	}

	void Logger::Info(const std::string& message, const std::string& category)
	{
		Log(LogLevel::INFO, message, category);
	}

	void Logger::Warning(const std::string& message, const std::string& category)
	{
		Log(LogLevel::WARNING, message, category);
	}

	void Logger::Error(const std::string& message, const std::string& category)
	{
		Log(LogLevel::ERROR, message, category);
	}

	void Logger::Fatal(const std::string& message, const std::string& category)
	{
		Log(LogLevel::FATAL, message, category);
	}

	void Logger::SetLogLevel(LogLevel level)
	{
		std::lock_guard<std::mutex> lock(logMutex);
		minLogLevel = level;
		Info("Log level set to " + LogLevelToString(level), "LOGGER");
	}

	void Logger::SetConsoleOutput(bool enabled)
	{
		std::lock_guard<std::mutex> lock(logMutex);
		consoleOutput = enabled;
		Info("Console output " + std::string(enabled ? "enabled" : "disabled"), "LOGGER");
	}

	void Logger::SetFileOutput(bool enabled)
	{
		std::lock_guard<std::mutex> lock(logMutex);
		fileOutput = enabled;
		Info("File output " + std::string(enabled ? "enabled" : "disabled"), "LOGGER");
	}

	std::string Logger::GetTimestamp()
	{
		auto now = std::chrono::system_clock::now();
		auto time_t = std::chrono::system_clock::to_time_t(now);
		auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(
			now.time_since_epoch()) % 1000;
		
		std::stringstream ss;
		tm localTime;
#ifdef _WIN32
		localtime_s(&localTime, &time_t);
#else
		localtime_r(&time_t, &localTime);
#endif
		ss << std::put_time(&localTime, "%Y-%m-%d %H:%M:%S");
		ss << '.' << std::setfill('0') << std::setw(3) << ms.count();
		
		return ss.str();
	}

	std::string Logger::LogLevelToString(LogLevel level)
	{
		switch (level)
		{
		case LogLevel::TRACE: return "TRACE";
		case LogLevel::DEBUG: return "DEBUG";
		case LogLevel::INFO: return "INFO";
		case LogLevel::WARNING: return "WARN";
		case LogLevel::ERROR: return "ERROR";
		case LogLevel::FATAL: return "FATAL";
		default: return "UNKNOWN";
		}
	}
}
