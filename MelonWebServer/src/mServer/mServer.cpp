#include "mServer.hpp"

mServer::mServer(const int32_t& port, std::shared_ptr<mLogger> const& logger)
{
	m_logger = logger;
	m_port = port;
	m_logger->Log("SERVER", "Creating Server...");
	m_flag = false;
	m_socket = std::make_unique<mSocket>(m_flag);
	if (m_socket->IsError())
	{
		m_logger->Log("SERVER", "Shutting down server.");
		throw;
	}

	m_logger->Log("SERVER", "Server is ready... Listening on port %i", m_port);
}

mServer::~mServer(void)
{
	m_logger->Log("SERVER", "Destroying Server");
}

void mServer::Listen()
{
	std::thread thread([this] { m_socket->Listen(); });

	m_flag = true;
	thread.join();
	

}

