#include "pch.h"
#include "altVMain.h"
#include "LuaScriptRuntime.h"


/* Entry Point for the Server */
EXPORT bool altMain(alt::IServer* server) {
	LuaScriptRuntime* luaScriptRuntime = new LuaScriptRuntime(server);
	server->RegisterScriptRuntime("lua", luaScriptRuntime);
	return true;
}

/* Return the SDK Version */
EXPORT uint32_t GetSDKVersion() {
	return ALTV_SDK_VERSION;
}