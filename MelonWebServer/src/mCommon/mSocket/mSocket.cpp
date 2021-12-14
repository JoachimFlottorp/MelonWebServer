#include "mSocket.hpp"

mSocket::mSocket(std::atomic_bool& cancellation_token)
{
	m_failure = false;
	m_result = nullptr;
	m_socket = INVALID_SOCKET;
	m_result = NULL;
	m_cancellation_token = &cancellation_token;
	
	Melon::logger->Log("SOCKET", "Creating a winsocket...");
#ifdef _WIN32
	if (WSAStartup(MAKEWORD(2, 2), &m_wsa) != 0) 
	{


		Melon::logger->Log("SOCKET", "Failure. Error Code: %d", WSAGetLastError());
		WSACleanup();
		m_failure = true;
		return;
	}
#endif // _WIN32

	ZeroMemory(&m_hints, sizeof(m_hints));
	m_hints.ai_family = AF_INET;
	m_hints.ai_socktype = SOCK_STREAM;
	m_hints.ai_protocol = IPPROTO_TCP;
	m_hints.ai_flags = AI_PASSIVE;

	if ((getaddrinfo(NULL, Melon::config->GetValue("PORT").c_str(), &m_hints, &m_result) != 0))
	{
#ifdef _WIN32
		Melon::logger->Log("SOCKET", "Error with getting address info. Error Code: %d", WSAGetLastError());
		WSACleanup(); 
#else
#endif // _WIN32
		m_failure = true;
		return;
	}

	if ((m_socket = socket(m_result->ai_family, m_result->ai_socktype, m_result->ai_protocol)) == INVALID_SOCKET)
	{
		freeaddrinfo(m_result);
#ifdef _WIN32
		Melon::logger->Log("SOCKET", "Failed creating socket. Error: %ld", WSAGetLastError());
		WSACleanup();
#else

#endif // _WIN32

		m_failure = true; 
		return;
	}

	if ((bind(m_socket, m_result->ai_addr, static_cast<int>(m_result->ai_addrlen)) == SOCKET_ERROR))
	{
		freeaddrinfo(m_result);
		closesocket(m_socket);
#ifdef _WIN32
		Melon::logger->Log("SOCKET", "Failed binding socket. Error: %d", WSAGetLastError());
		WSACleanup();
#else

#endif // _WIN32
		m_failure = true;
		return;
	}

	freeaddrinfo(m_result);

	if ((listen(m_socket, SOMAXCONN) == SOCKET_ERROR))
	{
		closesocket(m_socket);
#ifdef _WIN32
		Melon::logger->Log("SOCKET", "Failed listening on socket. Error: %d", WSAGetLastError());
		WSACleanup();
#else

#endif // _WIN32
		m_failure = true;
		return;
	}
}

mSocket::~mSocket(void)
{
	closesocket(m_socket);
#ifdef _WIN32
	WSACleanup();
#endif // _WIN32
}

bool mSocket::IsError()
{
	if (m_failure) return true;
	return false;
}

void mSocket::Listen()
{
	while (m_cancellation_token)
	{
		// Accept a client socket
		SOCKADDR_IN clientaddr{};
		int clientlen = sizeof(clientaddr);
		SOCKET client = accept(m_socket, reinterpret_cast<SOCKADDR*>(&clientaddr), &clientlen);
		if (client == INVALID_SOCKET) {
#ifdef _WIN32
			Melon::logger->Log("SOCKET", "Failed to accept socket. Error Code: %d", WSAGetLastError());
			WSACleanup();
#else

#endif // _WIN32
			closesocket(m_socket);
			return;
		}
		Melon::logger->Log("SOCKET", "Opened Connection to: %u.%u.%u.%u",
			clientaddr.sin_addr.S_un.S_un_b.s_b1,
			clientaddr.sin_addr.S_un.S_un_b.s_b2,
			clientaddr.sin_addr.S_un.S_un_b.s_b3,
			clientaddr.sin_addr.S_un.S_un_b.s_b4);
	

		// Receive until the peer shuts down the connection
		int iResult{};
		do {

			char recvbuf[SOCKET_BUFFER_SIZE];
			std::string sndbuf = "Hello, World!\n";
			int recvbuflen = SOCKET_BUFFER_SIZE;
			iResult = recv(client, recvbuf, recvbuflen, 0);
			if (iResult > 0) {
				Melon::logger->Log("SOCKET", "Bytes received: %d", iResult);
				Melon::logger->Log("SOCKET", "%s", recvbuf);

				if (send(client, sndbuf.c_str(), sndbuf.length(), 0) == SOCKET_ERROR)
				{
					closesocket(client);
#ifdef _WIN32
					Melon::logger->Log("SOCKET", "Sending to client failed. Error Code: %d", WSAGetLastError());
					WSACleanup();
#else

#endif // _WIN32
					m_failure = true;
					return;
				}
				else if (iResult == 0)
					Melon::logger->Log("SOCKET", "Connection closing...\n");
				else {
#ifdef _WIN32
					Melon::logger->Log("SOCKET", "recv failed with error: %d\n", WSAGetLastError());
					WSACleanup();
#else

#endif // _WIN32
					closesocket(client);
					return;
				}
			}

		} while (iResult > 0);

		// shutdown the connection since we're done
		if (shutdown(client, SD_SEND) == SOCKET_ERROR) {
			closesocket(client);
	#ifdef _WIN32
			Melon::logger->Log("SOCKET", "Client shutdown failed: Error Code: %d", WSAGetLastError());
			WSACleanup();
	#else

	#endif // _WIN32
		}
	}
}
