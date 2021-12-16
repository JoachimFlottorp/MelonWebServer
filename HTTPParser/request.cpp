#include "HTTP.hpp"

HTTP::request::request(const std::string& req)
{
	m_req = req;

	auto ss = std::stringstream(m_req);
	for (std::string line; std::getline(ss, line, '\n');)
	{
		m_req_vec.push_back(line);
	}

	if (m_req_vec.size() == 0)
	{
		m_error_list.push_back(HTTP::HEAD_ERROR::EMPTY);
		return;
	}

	// First line split between spaces. Contains method, path, version.
	std::vector<std::string> first_split = {};
	size_t pos = 0;

	while ((pos = m_req_vec[0].find(' ')) != std::string::npos)
	{
		std::string token = m_req_vec[0].substr(0, pos);
		if (token.length() > 0)
			first_split.push_back(token);
		// Visual studio recommended this, surely this works.
		m_req_vec[0].erase(0, static_cast<std::basic_string<char, std::char_traits<char>, std::allocator<char>>::size_type>(pos) + 1);
	}

	if (m_req_vec[0].length() > 0)
	{
		if (m_req_vec[0].back() == '\n' || m_req_vec[0].back() == '\r')
			m_req_vec[0].pop_back();
		first_split.push_back(m_req_vec[0]);
	}

	if (first_split.size() > 3 || first_split.size() < 3)
	{
		m_error_list.push_back(HTTP::HEAD_ERROR::WRONG_FORMATTED);
		return;
	}

	m_header.method = correct_method(first_split[0]);
	m_header.path = first_split[1];
	m_header.version = first_split[2];
	// Remove the first as we already parsed it and the rest of the data is "optional".
	m_req_vec.erase(m_req_vec.begin());
	printf("[Method]: %s\n", m_header.method.c_str());
	printf("[Path]: %s\n", m_header.path.c_str());
	printf("[Version]: %s\n", m_header.version.c_str());
	optionals_parse();
	request_body(); // [TODO]: Should i do something with this.

	// Not a valid request.
	if (!valid())
	{
		for (const auto& i : m_error_list)
			printf("[ERROR_REQUEST]: %i", i);
		return;
	}
	//for (const auto& i : m_header.optionals)
		//printf("%s : %s\n", i.first.c_str(), i.second.c_str());
}

HTTP::request::~request(void)
{

}

void HTTP::request::request_body()
{
	for (const auto& i : m_req_vec)
	{
		m_req_body.push_back(i);
	}
}

bool HTTP::request::check_error(HTTP::HEAD_ERROR error_type)
{
	for (auto const& i : m_error_list)
	{
		if (i == error_type)
			return true;
	}

	return false;
}

std::string HTTP::request::correct_method(const std::string& method)
{
	HTTP::method __met = HTTP::method();
	return __met.valid(method);
}

// Valid request
bool HTTP::request::valid()
{
	return m_error_list.size() > 0 ? false : true;
}


void HTTP::request::optionals_parse()
{
	std::vector<std::string> copy{};
	copy.assign(m_req_vec.begin(), m_req_vec.end());
	for (auto const& line : copy)
	{
		// [0] = key, [1] = value.
		std::vector<std::string> vec;

		auto first_semicolon = line.find_first_of(':');
		if (first_semicolon != std::string::npos)
		{
			vec.push_back(line.substr(0, first_semicolon));
			vec.push_back(line.substr(first_semicolon + 1));
		}

		// The newline at the bottom of header, which means the header is done.
		if (vec.size() <= 1)
		{
			m_req_vec.erase(m_req_vec.begin());
			return;
		}

		// Already exists and therefore the header is not correctly formatted.
		if (m_header.optionals.count(vec[0]) > 0)
		{
			m_error_list.push_back(HTTP::HEAD_ERROR::WRONG_FORMATTED);
			return;
		}

		m_header.optionals.insert(std::pair<std::string, std::string>(vec[0], vec[1]));
		m_req_vec.erase(m_req_vec.begin());
	}
}