#include "Logger.h"
#include <iostream>

namespace
{
auto g_minLevel = LogLevel::Info;
}

void LoggerInit(const LogLevel minLevel)
{
	g_minLevel = minLevel;
}

void LogWrite(const LogLevel level, const std::string& message)
{
	if (level < g_minLevel)
	{
		return;
	}

	const char* levelStr = nullptr;
	switch (level)
	{
	case LogLevel::Debug:
		levelStr = "DEBUG";
		break;
	case LogLevel::Info:
		levelStr = "INFO";
		break;
	case LogLevel::Warn:
		levelStr = "WARN";
		break;
	case LogLevel::Error:
		levelStr = "ERROR";
		break;
	}

	std::clog << "[" << levelStr << "] " << message << std::endl;
}

void LogDebug(const std::string& message)
{
	LogWrite(LogLevel::Debug, message);
}

void LogInfo(const std::string& message)
{
	LogWrite(LogLevel::Info, message);
}

void LogWarn(const std::string& message)
{
	LogWrite(LogLevel::Warn, message);
}

void LogError(const std::string& message)
{
	LogWrite(LogLevel::Error, message);
}