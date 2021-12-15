// HTTPParser.cpp : Defines the functions for the static library.
//

#include "pch.h"
#include "framework.h"
#include "HTTPParser.hpp"

HTTP::request HTTP::init_request(const std::string& request)
{
	HTTP::request __req = HTTP::request(request);
	return __req;
}