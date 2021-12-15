#include "main.hpp"
namespace Melon
{
	std::unique_ptr<mConfig> config;
	std::shared_ptr<mLogger> logger;
	std::unique_ptr<mServer> server;
}

void shutdown_sequence();

int main()
{
	std::atexit(shutdown_sequence);
	signal(SIGINT, [](int) {shutdown_sequence(); });
	try
	{
		Melon::config = std::make_unique<mConfig>("config.ini");
		Melon::logger = std::make_shared<mLogger>(std::stoi(Melon::config->GetValue("LOG_CONSOLE")));
		Melon::server = std::make_unique<mServer>(std::stoi(Melon::config->GetValue("PORT")), Melon::logger);
		Melon::server->Listen();
	}
	catch (const std::exception& e) 
	{
		std::cout << e.what() << std::endl;
		return -1;
	}
	return 0;
}

void shutdown_sequence()
{
	// Check if server is running.
	if (Melon::server)
	{
		if (!Melon::server->shutdown())
		{
			Melon::logger->Log("SERVER", "Shutdown sequence returned an error.");
		}
	}
	exit(0);
}