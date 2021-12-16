#pragma once

#include <vector>
#include <string>
#include <map>
#include <unordered_map>
#include <sstream>
#include <fstream>

namespace HTTP
{
	const std::string ERROR_PAGE =
		"<html>"
		"<head>"
		"<title>404 Page not found</title>"
		"</head>"
		"<body>"
		"<h1>404 Page not found.</h1>"
		"</body>"
		"</html>";

	class method
	{
	public:

		method() {};

		std::string valid(const std::string& m)
		{
			for (auto const& method : m_all_methods)
			{
				if (method.first == m)
					return method.second;
			}
			return std::string(" ");
		}

	private:

		// Unsure what exactly to put here, or if if honestly i just use an array here.
		std::map<std::string, std::string> m_all_methods 
		{
			{"GET", "GET"},
			{"HEAD", "HEAD"},
			{"POST", "POST"},
			{"PUT", "PUT"},
			{"DELETE", "DELETE"},
			{"CONNECT", "CONNECT"},
			{"OPTIONS", "OPTIONS"},
			{"TRACE", "TRACE"},
			{"PATCH", "PATCH"},
		};
	};

	// Use this to define a error the client has.
	enum class HEAD_ERROR
	{
		WRONG_METHOD,
		NO_PATH,
		UNKNOWN_VERSION,
		WRONG_FORMATTED,
		EMPTY,
	};

	struct header
	{
		std::string method;
		std::string path;
		std::string version;
		std::map<std::string, std::string> optionals;
	};

	struct webpage
	{
		int status_code;
		std::string page;
	};

	class request
	{
	public:
		request(const std::string& req);
		~request(void);

		std::string method();
		std::string path();
		std::string version();
		bool check_error(HTTP::HEAD_ERROR error_type);
		bool valid();
	private:
		std::string m_req;
		header m_header;
		std::vector<std::string> m_req_body;
		std::vector<std::string> m_req_vec;
		bool m_valid_method = true;
		std::vector<HTTP::HEAD_ERROR> m_error_list;

	private:
		void request_body();
		std::string correct_method(const std::string& method);
		void optionals_parse();
	}; // request.cpp

	class response
	{
	public:
		response(request& req, std::unordered_map<std::string, std::string>& files);
		~response(void);

		std::string connect();
	private:
		std::string m_status_line{};
		std::vector<std::string> m_header{};
		std::string m_body{};
		std::vector<std::string> m_all{};

	private:
		void construct_status_line(request& req, webpage& wp);
		void construct_headers(request& req, webpage& wp);
		void construct_body(request& req, webpage& wp);
		webpage load_page(std::string& page, std::unordered_map<std::string, std::string>& files);
	};
}
