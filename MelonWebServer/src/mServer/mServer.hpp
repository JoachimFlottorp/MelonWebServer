#pragma once

#include "../mCommon/mLogger/mLogger.hpp"
#include "../mCommon/mSocket/mSocket.hpp"

#include <memory>
#include <thread>
#include <chrono>

class mServer
{
public:
	mServer(const int32_t& port, std::shared_ptr<mLogger> const& logger);
	~mServer(void);

	void Listen();
private:
	int32_t m_port;
	// I doubt this is how you're supposed to do it. But it builds, so i'll keep going with this until i figure out shared_ptr.
	std::shared_ptr<mLogger> m_logger;
	std::unique_ptr<mSocket> m_socket;
	std::atomic_bool m_flag;
	//std::thread m_socket_thread;
};

