#pragma once

#ifdef _WIN32

#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <WinSock2.h>
#include <WS2tcpip.h>

// Need to link with Ws2_32.lib, Mswsock.lib, and Advapi32.lib
#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")

#else



#endif

#define SOCKET_BUFFER_SIZE 4096
#define FAILED_SOCKET 00001

#include <vector>

#include <HTTPParser.hpp>

#include "../Melon.hpp"
#include "../mLogger/mLogger.hpp"
#include "../mConfig/mConfig.hpp"


class mSocket
{
public:
	mSocket(std::atomic_bool& cancellation_token);
	~mSocket(void);

	bool IsError();
	void Listen();

private:
	bool m_failure;
	std::atomic_bool m_cancellation_token;
	SOCKET m_socket;
	struct addrinfo* m_result, m_hints;
#ifdef _WIN32
	WSADATA m_wsa;
	WSAPOLLFD m_fds;
#else
	pollfd m_fds[200];
#endif // _WIN32

private:
	SOCKET accept_connection();
	void handle_connection(const SOCKET& client);
	bool respond_client(const SOCKET& client, const std::string& response);
};

