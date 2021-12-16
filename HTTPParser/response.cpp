#include "HTTP.hpp"

template<typename ... Args>
std::string
str_frmt(const std::string& format, Args ... args);

HTTP::response::response(const request& req)
{
	construct_status_line(req); // [TODO]: Only does http:1.1
	construct_headers(req);
	construct_body(req);
	m_all.insert(std::end(m_all), std::begin(m_header), std::end(m_header));
	m_all.push_back("\n");
	if (m_body.size() > 0)
		m_all.insert(std::end(m_all), std::begin(m_body), std::end(m_body));
}

HTTP::response::~response(void)
{

}

void HTTP::response::construct_status_line(const request& req) // Simple for now, can't be asked anymore right now.
{
	m_status_line = str_frmt("%s %i %s", "HTTP/1.1", 200, "OK");
}

void HTTP::response::construct_headers(const request& req)
{
	m_header.push_back(str_frmt("Connection: %s", "close"));
	m_header.push_back(str_frmt("Server: %s", "Melon"));
	m_header.push_back(str_frmt("Access-Control-Allow-Origin: %s", "*"));
}

void HTTP::response::construct_body(const request& req)
{
	m_body.push_back(EXAMPLE_HTML);
}

std::string HTTP::response::connect()
{
	std::string str{};
	str += m_status_line + '\n';
	for (const auto& i : m_all)
	{

		str += i + (i != "\n" ? "\n" : std::string(""));
	}
	return str;
}

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