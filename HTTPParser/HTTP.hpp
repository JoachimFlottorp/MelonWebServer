#pragma once

#include <vector>
#include <string>
#include <map>

namespace HTTP
{
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

	class request
	{
	public:
		request(const std::string& req);
		~request(void);

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
		void parse_body();
		std::string correct_method(const std::string& method);
		void optionals_parse();
	}; // request.cpp

	request init_request(const std::string& request); // HTTPParser.cpp
}
