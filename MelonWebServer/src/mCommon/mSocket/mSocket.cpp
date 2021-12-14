#include "mSocket.hpp"

mSocket::mSocket()
{
	m_failure = false;
	m_result = nullptr;
#ifdef _WIN32
	m_socket = INVALID_SOCKET;
	m_result = nullptr;
	m_hints = nullptr;
	
	Melon::logger->Log("SOCKET", "Creating a winsocket...");
	if (WSAStartup(MAKEWORD(2, 2), &m_wsa) != 0) 
	{
		Melon::logger->Log("SOCKET", "Failure. Error Code: %d", WSAGetLastError());
		WSACleanup();
		m_failure = true;
		return;
	}

	ZeroMemory(&m_hints, sizeof(m_hints));
	m_hints->ai_family = AF_INET;
	m_hints->ai_socktype = SOCK_STREAM;
	m_hints->ai_protocol = IPPROTO_TCP;
	m_hints->ai_flags = AI_PASSIVE;

	if ((getaddrinfo(NULL, Melon::config->GetValue("PORT").c_str(), m_hints.get(), &m_result) != 0))
	{
		Melon::logger->Log("SOCKET", "Error with getting address info. Error Code: %d", WSAGetLastError());
		WSACleanup(); 
		m_failure = true;
		return;
	}

	if ((m_socket = socket(m_result->ai_family, m_result->ai_socktype, m_result->ai_protocol)) == INVALID_SOCKET)
	{
		Melon::logger->Log("SOCKET", "Failed creating socket. Error: %ld", WSAGetLastError());
		freeaddrinfo(m_result);
		WSACleanup();
		m_failure = true; 
		return;
	}

	if ((bind(m_socket, m_result->ai_addr, static_cast<int>(m_result->ai_addrlen)) == SOCKET_ERROR))
	{
		Melon::logger->Log("SOCKET", "Failed binding socket. Error: %d", WSAGetLastError());
		freeaddrinfo(m_result);
		closesocket(m_socket);
		WSACleanup();
		m_failure = true;
		return;
	}

	freeaddrinfo(m_result);

	if ((listen(m_socket, SOMAXCONN) == SOCKET_ERROR))
	{
		Melon::logger->Log("SOCKET", "Failed listening on socket. Error: %d", WSAGetLastError());
		closesocket(m_socket);
		WSACleanup();
		m_failure = true;
		return;
	}

#else

#endif // _WIN32

}

mSocket::~mSocket(void)
{
	closesocket(m_socket);
	WSACleanup();
}

bool mSocket::IsError()
{
	if (m_failure) return true;
	return false;
}

#ifdef _WIN32
void mSocket::Listen()
{
	// Accept a client socket
	SOCKADDR_IN clientaddr;
	int clientlen = sizeof(clientaddr);
	SOCKET client = accept(m_socket, reinterpret_cast<SOCKADDR*>(&clientaddr), &clientlen);
	if (client == INVALID_SOCKET) {
		Melon::logger->Log("SOCKET", "Failed to accept socket. Error Code: %d", WSAGetLastError());
		closesocket(m_socket);
		WSACleanup();
		return;
	}
	Melon::logger->Log("SOCKET", "Opened Connection to: %uc.%uc.%uc.%uc",
		clientaddr.sin_addr.S_un.S_un_b.s_b1,
		clientaddr.sin_addr.S_un.S_un_b.s_b2,
		clientaddr.sin_addr.S_un.S_un_b.s_b3,
		clientaddr.sin_addr.S_un.S_un_b.s_b4);
	

	// Receive until the peer shuts down the connection
	int iResult{};
	do {

		char recvbuf[512];
		int recvbuflen = 512;
		iResult = recv(client, recvbuf, recvbuflen, 0);
		if (iResult > 0) {
			printf("Bytes received: %d\n", iResult);

			// Echo the buffer back to the sender
			int iSendResult = send(client, recvbuf, iResult, 0);
			if (iSendResult == SOCKET_ERROR) {
				printf("send failed with error: %d\n", WSAGetLastError());
				closesocket(client);
				WSACleanup();
				return;
			}
			printf("Bytes sent: %d\n", iSendResult);
			}
		else if (iResult == 0)
			printf("Connection closing...\n");
		else {
			printf("recv failed with error: %d\n", WSAGetLastError());
			closesocket(client);
			WSACleanup();
			return;
		}

		} while (iResult > 0);

	// shutdown the connection since we're done
	if (shutdown(client, SD_SEND) == SOCKET_ERROR) {
		Melon::logger->Log("SOCKET", "Client shutdown failed: Error Code: %d", WSAGetLastError());
		closesocket(client);
		WSACleanup();
		return;
	}
}
#else

void mSocket::Listen()
{

}

#endif // _WIN32