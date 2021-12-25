#include "mServer.hpp"

std::string name(char* a, size_t size);

mServer::mServer(const int32_t& port, std::shared_ptr<mLogger> const& logger)
{
	m_logger = logger;
	m_port = port;
	m_logger->Log("SERVER", "Creating Server...");
	m_close_socket = false;
	std::unordered_map<std::string, std::string> files = cache_pages();

	m_socket = std::make_unique<mSocket>(m_close_socket, files);
	if (m_socket->IsError())
	{
		m_logger->Log("SERVER", "Shutting down server.");
		throw;
	}

	m_logger->Log("SERVER", "Server is ready... Listening on port %i", m_port);
}

mServer::~mServer(void)
{
	m_logger->Log("SERVER", "Destroying Server...");
}

void mServer::Listen()
{
	std::thread thread([this] { m_socket->Listen(); });

	while (!m_socket->IsError())
	{

	}

	thread.join();
}

bool mServer::shutdown()
{
	// Tell the socket thread to close.
	m_close_socket = true;

	return true;
}

std::unordered_map<std::string, std::string> mServer::cache_pages()
{
	std::unordered_map<std::string, std::string> cached_files;
	std::string file_location = Melon::config->GetValue("HTTP_LOCATION");
	// Using c++14 in this project, so gotta use dirent.h

	std::vector<std::string> directory;
	struct dirent* entry;
	DIR* dir = opendir(file_location.c_str());

	// Directory contains nothing.
	if (dir == NULL)
	{
		cached_files.insert(std::make_pair(std::string(""), std::string("")));
		closedir(dir);
		return cached_files;
	}
	while ((entry = readdir(dir)) != NULL)
	{
		std::string fname = name(entry->d_name, entry->d_namlen);
		std::string full_path = file_location + "/" + entry->d_name;
		std::ifstream ifs(full_path);

		if (entry->d_name != ".")
		{
			if (!ifs.is_open())
			{
				Melon::logger->Log("CACHING", "Supposed file: [%s] could not be opened.", full_path.c_str());
			}
			else
			{
				auto contents = std::string(std::istreambuf_iterator<char>(ifs), std::istreambuf_iterator<char>());

				if (fname == "index" || fname == "index.html")
					cached_files.insert(std::make_pair("/", contents));
				else
					cached_files.insert(std::make_pair('/'+fname, contents));
			}
		}
	}
	closedir(dir);
	return cached_files;
}

std::string name(char* a, size_t size)
{
	std::string s{};
	for (auto i = 0; i < size; i++)
	{
		s += a[i];
	}
	return s;
}