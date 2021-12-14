#include "mServer.hpp"

mServer::mServer(const int32_t& port, std::shared_ptr<mLogger> const& logger)
{
	m_logger->Log("SERVER", "Creating Server...");
	m_port = port;
	m_logger = logger;
	m_socket = std::make_unique<mSocket>();
	if (m_socket->IsError())
	{
		m_logger->Log("SERVER", "Shutting down server.");
		throw;
	}

	m_logger->Log("SERVER", "Server is ready...");
}

mServer::~mServer(void)
{
	m_logger->Log("SERVER", "Destroying Server");
}

void mServer::Listen()
{
	m_socket->Listen();
}