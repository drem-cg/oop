#include "Logger.h"

std::ostream* Logger::m_output = &std::clog;
// TODO: инит логера с конкретным св-вом

void Logger::Info(const std::string& message)
{
	*m_output << "[INFO] " << message << std::endl;
}

void Logger::Error(const std::string& message)
{
	*m_output << "[ERROR] " << message << std::endl;
}