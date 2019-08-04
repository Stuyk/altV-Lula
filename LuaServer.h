#pragma once
#define ALT_SERVER_API
#include <SDK.h>

class LuaServer
{
	public:
		LuaServer(alt::IServer* server);

		uint32_t GetHash(std::string& value);

		template<typename T>
		void LogServer(T value) {
			server->LogInfo(value);
		}

		void LogError(std::string& value);

	private:
		alt::IServer* server = nullptr;
};