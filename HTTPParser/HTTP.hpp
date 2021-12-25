#pragma once

#include <vector>
#include <string>
#include <map>
#include <unordered_map>
#include <sstream>
#include <fstream>
#include <functional>

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
				if (method == m)
					return method;
			}
			return std::string(" ");
		}

	private:

		std::vector<std::string> m_all_methods
		{
			"GET",
			"HEAD", 
			"POST", 
			"PUT", 
			"DELETE", 
			"CONNECT",
			"OPTIONS",
			"TRACE",
			"PATCH",
		};
	};

	enum class STATUS_CODE // As if im implementing all of these lol.
	{
		// Information
		CONTINUE = 100,
		SWITCHING_PROTOCOLS = 101,
		PROCESSING = 102,
		EARLY_HINTS = 103,

		// Successfull
		OK = 200,
		CREATED = 201,
		ACCEPTED = 202,
		NON_AUTHOROTIVE_INFORMATION = 203,
		NO_CONTENT = 204,
		RESET_CONTENT = 205,
		PARTIAL_CONTENT = 206,
		MULTI_STATUS= 207,
		ALREADY_REPORTED= 208,
		IM_USED= 209,

		// Redirection
		MULTIPLE_CHOICES= 300,
		MOVED_PERMAMENTLY= 301,
		FOUND = 302,
		SEE_OTHER= 303,
		NOT_MODIFIED = 304,
		TEMPORARY_REDIRECTION = 307,
		PERMAMENT_REDIRECTION = 308,

		// Client error
		BAD_REQUEST = 400,
		UNAUTHORIZED = 401,
		PAYMENT_REQUIRED = 402,
		FORBIDDEN = 403,
		NOT_FOUND = 404,
		METHOD_NOT_ALLOWED = 405,
		NOT_ACCEPTABLE = 406,
		PROXY_AUTHENTICATION_REQUIRED = 407,
		REQUEST_TIMEOUT = 408,
		CONFLICT = 409,
		GONE = 410,
		LENGTH_REQUIRED = 411,
		PRECONDITION_FAILED = 412,
		PAYLOAD_TOO_LARGE = 413,
		URI_TOO_LONG = 414,
		UNSUPPORTED_MEDIA_TYPe = 415,
		RANGE_NOT_SATISFIABLE = 416,
		EXPECTATION_FAILED = 417,
		IM_A_TEAPOT = 418,
		UNPROCESSABLE_ENTITY = 422,
		LOCKED = 423,
		FAILED_DEPENDENCY = 424,
		TOO_EARLY = 425,
		UPGRADE_REQUIRED = 426,
		PRECONDITION_REQUIRED = 427,
		TOO_MANY_REQUESTS = 429,
		REQUEST_HEADER_FIELDS_TOO_LARGE = 431,
		UNAVAILABLE_FOR_LEGAL_REASONS = 451,

		// Server eror
		INTERNAL_SERVER_ERROR = 500,
		NOT_IMPLEMENTED = 501,
		BAD_GATEWAY = 502,
		SERVICE_UNAVAILABLE = 503,
		GATEWAY_TIMEOUT = 504,
		HTTP_VERSION_NOT_SUPPORTED = 505,
		VARIANT_ALSO_NEGOTIATES = 506,
		INSUFFICENT_STORAGE = 507,
		LOOP_DETECTED = 508,
		NOT_EXTENDED = 510,
		NETWORK_AUTHENTICATION_REQUIRED = 501
	};

	static std::map<STATUS_CODE, std::string> res_status_message
	{
		{ STATUS_CODE::OK, "OK"},
		{ STATUS_CODE::NOT_FOUND, "Not Found"}
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
		STATUS_CODE status_code;
		std::string status_message;
		std::string page;

		void set_sm()
		{
			for (const auto& code : HTTP::res_status_message)
			{
				if (code.first == status_code)
				{
					status_message = code.second;
					break;
				}
			}
		}
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
	}; // response.cpp
}
