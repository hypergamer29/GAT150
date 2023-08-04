#pragma once
#include <string>
#include <cassert>
#include <fstream>

#define INFO_LOG    { Loki::g_logger.Log(Loki::LogLevel::Info, __FILE__, __LINE__); }
#define WARNING_LOG { Loki::g_logger.Log(Loki::LogLevel::Warning, __FILE__, __LINE__); }
#define ERROR_LOG   { Loki::g_logger.Log(Loki::LogLevel::Error, __FILE__, __LINE__); }
#define ASSERT_LOG  { Loki::g_logger.Log(Loki::LogLevel::Assert, __FILE__, __LINE__); }

namespace Loki {
	enum class LogLevel {
		Info,
		Warning,
		Error,
		Assert
	};

	class Logger {
	public: 
		Logger(LogLevel logLevel,std::ostream* ostream) : 
			m_logLevel { logLevel },
			m_ostream{ ostream }  
		{}

		bool Log(LogLevel logLevel, const std::string& filenmae, int line);

	private:
		LogLevel m_logLevel;
		std::ostream* m_ostream = nullptr;
		std::ofstream m_ofstream;
	};

	extern Logger g_logger;
}