#pragma once
#define ALT_SERVER_API
#include <SDK.h>

using namespace alt;

class LuaScriptRuntime : public alt::IScriptRuntime
{
	alt::IResource* CreateResource(alt::IResource::CreationInfo* info) override;
	void RemoveResource(alt::IResource* resource) override;

	void OnTick() override;

	public:
		LuaScriptRuntime(alt::IServer* server);

	private:
		alt::IServer* server;
};

