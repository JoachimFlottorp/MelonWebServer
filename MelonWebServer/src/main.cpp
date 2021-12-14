#include "main.hpp"
namespace Melon
{
	std::unique_ptr<mConfig> config;
	std::shared_ptr<mLogger> logger;
	std::unique_ptr<mServer> server;
}
int main()
{
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