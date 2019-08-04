#include "pch.h"
#include "LuaServer.h"

LuaServer::LuaServer(alt::IServer* server) {
	this->server = server;
}

uint32_t LuaServer::GetHash(std::string& value)
{
	return server->Hash(value);
}

/*
void LuaServer::LogServer(std::string& value)
{
	server->LogInfo(value);
}
*/

void LuaServer::LogError(std::string& value)
{
	server->LogError(value);
}