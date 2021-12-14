#pragma once

#include <memory>

class mConfig;
class mLogger;
class mServer;

namespace Melon
{
	extern std::unique_ptr<mConfig> config;
	extern std::shared_ptr<mLogger> logger;
	extern std::unique_ptr<mServer> server;
}