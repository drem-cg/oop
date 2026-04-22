#include "../../include/Logger/Logger.h"

#include <iostream>

namespace
{
auto defType = LogType::Info;
} // namespace

void LoggerInit(const LogType minLevel)
{
	defType = minLevel;
}

void LogWrite(const LogType type, const std::string& message)
{
	if (type < defType)
	{
		return;
	}

	const char* levelStr = nullptr;
	switch (type)
	{
	case LogType::Debug:
		levelStr = "DEBUG";
		break;
	case LogType::Info:
		levelStr = "INFO";
		break;
	case LogType::Warn:
		levelStr = "WARN";
		break;
	case LogType::Error:
		levelStr = "ERROR";
		break;
	}

	std::cout << "[" << levelStr << "] " << message << std::endl;
}

void LogDebug(const std::string& message)
{
	LogWrite(LogType::Debug, message);
}

void LogInfo(const std::string& message)
{
	LogWrite(LogType::Info, message);
}

void LogWarn(const std::string& message)
{
	LogWrite(LogType::Warn, message);
}

void LogError(const std::string& message)
{
	LogWrite(LogType::Error, message);
}