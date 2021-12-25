#include "HTTP.hpp"

template<typename ... Args>
std::string
str_frmt(const std::string& format, Args ... args);

std::string no_ext(std::string& page);

HTTP::response::response(request& req, std::unordered_map<std::string, std::string>& files)
{
	std::string path = req.path();
	HTTP::webpage wp = load_page(path, files);

	construct_status_line(req, wp); // [TODO]: Only does http:1.1
	construct_headers(req, wp);
	construct_body(req, wp);
	m_all.insert(std::end(m_all), std::begin(m_header), std::end(m_header));
	m_all.push_back("\n");
	if (m_body.size() > 0)
		m_all.insert(std::end(m_all), m_body);
}

HTTP::response::~response(void)
{

}

void HTTP::response::construct_status_line(request& req, webpage& wp)
{
	m_status_line = str_frmt("%s %i %s", "HTTP/1.1", wp.status_code, wp.status_message.c_str());
}

void HTTP::response::construct_headers(request& req, webpage& wp)
{
	m_header.push_back(str_frmt("Connection: %s", "close"));
	m_header.push_back(str_frmt("Server: %s", "Melon"));
	m_header.push_back(str_frmt("Access-Control-Allow-Origin: %s", "*"));
}

void HTTP::response::construct_body(request& req, webpage& wp)
{
	m_body = wp.page;
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

HTTP::webpage HTTP::response::load_page(std::string& page, std::unordered_map<std::string, std::string>& files)
{
	webpage wp;
	auto it = files.find(page);
	auto two = files.find(no_ext(page));
	if (it == files.end() || two == files.end())
	{
		wp.page = ERROR_PAGE;
		wp.status_code = HTTP::STATUS_CODE::NOT_FOUND;
		wp.set_sm();
		return wp;
	}
	wp.page = it->second;
	wp.status_code = HTTP::STATUS_CODE::OK;
	wp.set_sm();
	return wp;
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

std::string no_ext(std::string& page)
{
	auto first_period = page.find_first_of('.');
	return page.substr(0, first_period || page.length());
}