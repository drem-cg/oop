#pragma once

#include <string>

enum class LogType
{
	Debug,
	Info,
	Warn,
	Error
};

void LoggerInit(LogType defType = LogType::Info);
void LogWrite(LogType type, const std::string& message);

void LogDebug(const std::string& message);
void LogInfo(const std::string& message);
void LogWarn(const std::string& message);
void LogError(const std::string& message);