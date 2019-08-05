#pragma once
#define ALT_SERVER_API
#include <SDK.h>

#include <sol.hpp>
#ifdef _WIN32
#pragma comment(lib, "liblua53.a")
#endif

class LuaLoader
{
	public:
		sol::state lua;

		std::list<alt::IPlayer*> currentPlayers;

		LuaLoader(alt::IServer* server, alt::IResource::CreationInfo* info);

		// Main Resource Files
		void LoadPackages(alt::IResource::CreationInfo* info);

		void LoadMainResourceFile(alt::IResource::CreationInfo* info);

		// Binding Class Functions
		void BindIPlayer();

		void BindIServer(sol::table alt);

		void BindColshapes();

		uint32_t GetHash(std::string& value);

		void LogServer(std::string& value);

		void LogError(std::string& value);

		std::list<alt::IPlayer*> GetPlayersByName(std::string name);

	private:
		alt::IServer* server = nullptr;
		
};