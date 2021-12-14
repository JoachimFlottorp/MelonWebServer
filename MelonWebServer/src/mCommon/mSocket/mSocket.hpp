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

#include "../Melon.hpp"
#include "../mLogger/mLogger.hpp"
#include "../mConfig/mConfig.hpp"

class mSocket
{
public:
	mSocket();
	~mSocket(void);

	bool IsError();
	void Listen();

private:
	bool m_failure;
#ifdef _WIN32
	WSADATA m_wsa;
	SOCKET m_socket;
	std::unique_ptr<struct addrinfo>(m_hints);
	struct addrinfo* m_result;
#else

#endif // _WIN32
};

