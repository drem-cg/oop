#include "Logger.h"

#include <chrono>
#include <cstdio>
#include <iostream>
#include <mutex>

namespace
{
LogLevel g_minLevel = LogLevel::Info;
std::mutex g_mutex;

std::string GetTimestamp()
{
	static auto startTime = std::chrono::steady_clock::now();
	const auto now = std::chrono::steady_clock::now();
	const auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(now - startTime).count();

	const unsigned long seconds = static_cast<unsigned long>(ms / 1000);
	const unsigned int remainingMs = static_cast<unsigned int>(ms % 1000);

	const unsigned long minutes = seconds / 60;
	const unsigned long secs = seconds % 60;
	const unsigned long hours = minutes / 60;
	const unsigned int mins = static_cast<unsigned int>(minutes % 60);

	char buf[16];
	std::snprintf(buf, sizeof(buf), "%02lu:%02u:%02lu.%03u",
		hours, mins, secs, remainingMs);
	return buf;
}
} // namespace

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

	std::lock_guard<std::mutex> lock(g_mutex);
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

	std::cerr << "[" << GetTimestamp() << "] [" << levelStr << "] " << message << std::endl;
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