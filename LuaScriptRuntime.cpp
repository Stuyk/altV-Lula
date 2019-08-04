#include "pch.h"
#include "LuaScriptRuntime.h"
#include "LuaResource.h"

alt::IResource* LuaScriptRuntime::CreateResource(alt::IResource::CreationInfo* info)
{
	server->LogDebug(info->name);
	auto* luaResource = new LuaResource(this->server, info);
	return luaResource;
}

void LuaScriptRuntime::RemoveResource(alt::IResource* resource)
{
	// Handle removing resource.
	delete resource;
}

LuaScriptRuntime::LuaScriptRuntime(alt::IServer* server)
{
	this->server = server;
}

void LuaScriptRuntime::OnTick() { }

