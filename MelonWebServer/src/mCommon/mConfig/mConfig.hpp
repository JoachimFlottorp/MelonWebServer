#pragma once

#include <fstream>
#include <string>
#include <unordered_map>
#include <sstream>
#include <exception>

const std::string WHITESPACE = " \n\r\t\f\v";

class mConfig
{
public:
	mConfig(const std::string& filename);
	~mConfig(void);

	const std::string& GetValue(std::string key);

private:
	void ProcessLine(const std::string& line);
	std::string RTrim(const std::string& s);
	std::string LTrim(const std::string& s);

private:
	std::unordered_map<std::string, std::string> m_values;
	std::string m_emptystring;
};

