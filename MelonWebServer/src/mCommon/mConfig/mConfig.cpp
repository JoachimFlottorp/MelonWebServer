#include "mConfig.hpp"
#include <iostream>

mConfig::mConfig(const std::string& filename)
{
	m_emptystring = "";
	std::ifstream in(filename);
	if (!in)
	{
		throw std::system_error(errno, std::system_category(), "failed to open " + filename);
	}

	std::string line;
	while (std::getline(in, line))
	{
		if (line.length() > 0)
		{
			if (line[0] != '#') ProcessLine(line);
		}
	}
}

mConfig::~mConfig(void)
{

}

const std::string& mConfig::GetValue(std::string key)
{
	for (auto const& i : m_values)
	{
		if (i.first == key) return i.second;
	}
	return m_emptystring;
}

void mConfig::ProcessLine(const std::string& line)
{
	std::stringstream ss(line);
	std::string segment;
	std::vector<std::string> list;

	while (std::getline(ss, segment, '='))
	{
		list.push_back(mConfig::RTrim(mConfig::LTrim(segment)));
	}

	if (list.size() != 2) return;

	m_values.insert({ list[0], list[1] });
	return;
}

std::string mConfig::LTrim(const std::string& s)
{
	size_t start = s.find_first_not_of(WHITESPACE);
	return (start == std::string::npos) ? "" : s.substr(start);
}

std::string mConfig::RTrim(const std::string& s)
{
	size_t end = s.find_last_not_of(WHITESPACE);
	return (end == std::string::npos) ? "" : s.substr(0, end + 1);
}