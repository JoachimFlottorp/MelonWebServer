#pragma once

#pragma warning(disable : 4996) //_CRT_SECURE_NO_WARNIN

#include <string>
#include <iostream>
#include <memory>
#include <stdexcept>

class mLogger
{
public:
	mLogger(const bool& log_console)
	{
		m_log_console = log_console;
		a_a = 9999;
	}
	~mLogger(void)
	{

	}

	template<typename ... Args>
	void 
	Log(const std::string& category, const std::string& format, Args ... args)
	{
		std::time_t t = std::time(nullptr);
		char timeStr[100];
		std::strftime(timeStr, sizeof(timeStr), "%d-%m-%y %H:%M:%S", std::localtime(&t));
		if (m_log_console) std::cout << "[" << category << "] " << "[" << timeStr << "]: " << mLogger::str_frmt(format, args ...) << std::endl;
		return;
	}

private:
	int a_a;
	bool m_log_console;

private:
	template<typename ... Args>
	std::string
	str_frmt(const std::string& format, Args ... args)
	{
		int size_s = std::snprintf(nullptr, 0, format.c_str(), args ...) + 1;
		if (size_s <= 0) { return "Error during formatting"; }
		auto size = static_cast<size_t>(size_s);
		auto buf = std::make_unique<char[]>(size);
		std::snprintf(buf.get(), size, format.c_str(), args ...);
		return std::string(buf.get(), buf.get() + size - 1);
	}
};