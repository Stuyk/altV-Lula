#include "pch.h"
#include "LuaResource.h"
#include "LuaLoader.h"

LuaLoader* luaLoader;
alt::IServer* currServer = nullptr;
auto resourcesCache = new alt::Array<LuaResource*>;

LuaResource::LuaResource(alt::IServer* server, alt::IResource::CreationInfo* info) : alt::IResource(info)
{
	// Open Lua Libraries
	

	this->server = server;
	resourcesCache->Push(this);
	currServer = server;

	// Setup LuaLoader
	luaLoader = new LuaLoader(server, info);

	

	//lua["onPlayerConnect"] = new sol::table(lua.lua_state(), sol::new_table());
}

LuaResource::~LuaResource()
{
	/*
	int i = 0;
	for (auto resource : *resourcesCache) {
		if (resource == this) {
			auto newResourcesCache = new alt::Array<LuaResource*>;
			for (auto cloneResource : *resourcesCache) {
				if (cloneResource != this) {
					newResourcesCache->Push(cloneResource);
				}
			}
			free(resourcesCache);
			resourcesCache = newResourcesCache;
			break;
		}
		i++;
	}
	*/
}

void LuaResource_SetExport(LuaResource* resource, const char* key, const alt::MValue& val) {
	//resource->SetExport(key, val);
}

void LuaResource::MakeClient(alt::IResource::CreationInfo* info, alt::Array<alt::String> files) {
	//info->type = "lua";
}

bool LuaResource::OnEvent(const alt::CEvent* ev)
{
	if (ev == nullptr) return true;
	switch (ev->GetType()) {
		case alt::CEvent::Type::PLAYER_CONNECT:
			{
				sol::state lua;
				auto conPlayer = reinterpret_cast<const alt::CPlayerConnectEvent*>(ev)->GetTarget();
				luaLoader->currentPlayers.push_back(conPlayer);
				std::vector<sol::function>& refToEvents = luaLoader->lua["PlayerConnectEvent"];
				for (const auto& func : refToEvents) {
					func(conPlayer);
				}
			}
			break;

		case alt::CEvent::Type::PLAYER_DISCONNECT:
			{
				sol::state lua;
				auto dcPlayer = reinterpret_cast<const alt::CPlayerDisconnectEvent*>(ev)->GetTarget();
				luaLoader->currentPlayers.remove(dcPlayer);
				std::vector<sol::function>& refToEvents = luaLoader->lua["PlayerDisconnectEvent"];
				for (const auto& func : refToEvents) {
					func(dcPlayer);
				}
			}
			break;

		default:
			break;
	}


	return true;
}
