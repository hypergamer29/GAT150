#pragma once
#include "Framework/Singleton.h"
#include <string>
#include <cassert>
#include <fstream>
#include <iostream>


#define INFO_LOG(message)    { Loki::Logger::Instance().Log(Loki::LogLevel::Info, __FILE__, __LINE__); }
#define WARNING_LOG(message) { Loki::Logger::Instance().Log(Loki::LogLevel::Warning, __FILE__, __LINE__); }
#define ERROR_LOG(message)   { Loki::Logger::Instance().Log(Loki::LogLevel::Error, __FILE__, __LINE__); }
#define ASSERT_LOG(condition, message)  { if (!condition && Loki::Logger::Instance().Log(Loki::LogLevel::Assert, __FILE__, __LINE__); }

namespace Loki {
	enum class LogLevel {
		Info,
		Warning,
		Error,
		Assert
	};

	class Logger : public Singleton<Logger> {
	public: 
		Logger(LogLevel logLevel = LogLevel::Info,std::ostream* ostream = &std::cout, const std::string& filename = "log.txt") :
			m_logLevel { logLevel },
			m_ostream{ ostream }  
		{
			if (!filename.empty()) m_ofstream.open(filename);
		}

		bool Log(LogLevel logLevel, const std::string& filenmae, int line);

		template<typename T>
		Logger& operator << (T vaule);

	private:
		LogLevel m_logLevel;
		std::ostream* m_ostream = nullptr;
		std::ofstream m_ofstream;
	};

	template<typename T>
	inline Logger& Logger::operator<<(T value) {

	}
}