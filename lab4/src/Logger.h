#pragma once

#include <string>

enum class LogLevel
{
	Debug,
	Info,
	Warn,
	Error
};

void LoggerInit(LogLevel minLevel = LogLevel::Info);
void LogWrite(LogLevel level, const std::string& message);

void LogDebug(const std::string& message);
void LogInfo(const std::string& message);
void LogWarn(const std::string& message);
void LogError(const std::string& message);