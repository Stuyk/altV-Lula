#include "pch.h"
#include "LuaResource.h"
#include "LuaServer.h"

alt::IServer* currServer = nullptr;
auto resourcesCache = new alt::Array<LuaResource*>;
sol::state lua;

LuaResource::LuaResource(alt::IServer* server, alt::IResource::CreationInfo* info) : alt::IResource(info)
{
	// Open Lua Libraries
	lua.open_libraries(sol::lib::base, sol::lib::package);

	this->server = server;
	resourcesCache->Push(this);
	currServer = server;

	// Setup LuaServer Pointer?
	LuaServer* luaServer = new LuaServer(server);

	// Setup Namespace for alt
	auto alt = lua["alt"].get_or_create<sol::table>();

	// IPlayer
	sol::usertype<alt::IPlayer> player_type = lua.new_usertype<alt::IPlayer>("Player");
	player_type["spawn"] = &alt::IPlayer::Spawn;
	player_type["despawn"] = &alt::IPlayer::Despawn;
	player_type["getName"] = sol::readonly_property(&alt::IPlayer::GetName);
	player_type["getSocialID"] = sol::readonly_property(&alt::IPlayer::GetSocialID);
	player_type["getHwidHash"] = sol::readonly_property(&alt::IPlayer::GetHwidHash);
	player_type["getHwidExHash"] = sol::readonly_property(&alt::IPlayer::GetHwidExHash);
	player_type["getAuthToken"] = sol::readonly_property(&alt::IPlayer::GetAuthToken);
	player_type["health"] = sol::property(&alt::IPlayer::GetHealth, &alt::IPlayer::SetHealth);
	player_type["model"] = sol::property(&alt::IPlayer::GetModel, &alt::IPlayer::SetModel);
	player_type["maxhealth"] = sol::property(&alt::IPlayer::GetMaxHealth, &alt::IPlayer::SetMaxHealth);
	player_type["giveWeapon"] = &alt::IPlayer::GiveWeapon;
	player_type["removeWeapon"] = &alt::IPlayer::RemoveWeapon;
	player_type["removeAllWeapons"] = &alt::IPlayer::RemoveAllWeapons;
	player_type["addWeaponComponent"] = &alt::IPlayer::AddWeaponComponent;
	player_type["removeWeaponComponent"] = &alt::IPlayer::RemoveWeaponComponent;
	player_type["getCurrentWeaponComponents"] = sol::readonly_property(&alt::IPlayer::GetCurrentWeaponComponents);
	player_type["setWeaponTintIndex"] = &alt::IPlayer::SetWeaponTintIndex;
	player_type["getCurrentWeaponTintIndex"] = sol::readonly_property(&alt::IPlayer::GetCurrentWeaponTintIndex);
	player_type["weapon"] = sol::property(&alt::IPlayer::GetCurrentWeapon, &alt::IPlayer::SetCurrentWeapon);
	player_type["isDead"] = sol::readonly_property(&alt::IPlayer::IsDead);
	player_type["isJumping"] = sol::readonly_property(&alt::IPlayer::IsJumping);
	player_type["isInRagdoll"] = sol::readonly_property(&alt::IPlayer::IsInRagdoll);
	player_type["isAiming"] = sol::readonly_property(&alt::IPlayer::IsAiming);
	player_type["isShooting"] = sol::readonly_property(&alt::IPlayer::IsShooting);
	player_type["isReloading"] = sol::readonly_property(&alt::IPlayer::IsReloading);
	player_type["armor"] = sol::property(&alt::IPlayer::GetArmour, &alt::IPlayer::SetArmour);
	player_type["maxArmor"] = sol::property(&alt::IPlayer::GetMaxArmour, &alt::IPlayer::SetMaxArmour);
	player_type["moveSpeed"] = &alt::IPlayer::GetMoveSpeed;
	player_type["getAmmo"] = sol::readonly_property(&alt::IPlayer::GetAmmo);
	player_type["getAimPos"] = sol::readonly_property(&alt::IPlayer::GetAimPos);
	player_type["getHeadRotation"] = sol::readonly_property(&alt::IPlayer::GetHeadRotation);
	player_type["isInVehicle"] = sol::readonly_property(&alt::IPlayer::IsInVehicle);
	player_type["getVehicle"] = sol::readonly_property(&alt::IPlayer::GetVehicle);
	player_type["getSeat"] = sol::readonly_property(&alt::IPlayer::GetSeat);
	player_type["getEntityAimingAt"] = sol::readonly_property(&alt::IPlayer::GetEntityAimingAt);
	player_type["getEntityAimingOffset"] = sol::readonly_property(&alt::IPlayer::GetEntityAimOffset);
	player_type["isFlashlightActive"] = sol::readonly_property(&alt::IPlayer::IsFlashlightActive);
	player_type["kick"] = &alt::IPlayer::Kick;

	// alt. Functions
	alt.set_function("hash", &LuaServer::GetHash, luaServer);
	alt.set_function("log", &LuaServer::LogServer<std::string>, luaServer);
	alt.set_function("logError", &LuaServer::LogError, luaServer);
	
	// alt.get
	alt.set_function("getVehicles", &alt::IServer::GetVehicles, server);
	alt.set_function("getPlayers", &alt::IServer::GetPlayers, server);
	alt.set_function("getPlayersByName", &alt::IServer::GetPlayersByName, server);
	alt.set_function("getEntityByID", &alt::IServer::GetEntityByID, server);
	
	// alt.create
	alt.set_function("createVehicle", &alt::IServer::CreateVehicle, server);
	alt.set_function("createColshapeCylinder", &alt::IServer::CreateColShapeCylinder, server);
	alt.set_function("createColshapeSphere", &alt::IServer::CreateColShapeSphere, server);
	alt.set_function("createColShapeCircle", &alt::IServer::CreateColShapeCircle, server);
	alt.set_function("createColShapeCube", &alt::IServer::CreateColShapeCube, server);
	alt.set_function("createColShapeRectangle", &alt::IServer::CreateColShapeRectangle, server);
	
	


	alt.new_usertype<alt::StringView>("StringView",
		sol::constructors<alt::StringView(std::string)>(),
		"ToString", &alt::StringView::ToString
	);

	// ColShape
	/*
	alt.new_usertype<alt::IColShape>("ColShape",
		"GetPosition", &alt::IColShape::GetPosition,
		"GetDimension", &alt::IColShape::GetDimension,
		"SetDimension", &alt::IColShape::SetDimension,
		"SetPosition", &alt::IColShape::SetPosition,
		"Set"
	);
	*/
	
	// Position
	//alt.new_usertype<alt::Position>("Position", )


	alt.new_usertype<alt::Position>("Position",
		sol::constructors<alt::Position(), // Default Constructor
		alt::Position(float, float, float)>(), // Constructor
		// Vector3 Properties
		"x", &alt::Position::x,
		"y", &alt::Position::y,
		"z", &alt::Position::z);


	//alt.new_usertype<alt::Position>(");

	// Rotation
	alt.new_usertype<alt::Rotation>("Rotation",
		sol::constructors<alt::Rotation(), alt::Rotation(float, float, float)>(),
		"x", &alt::Rotation::roll,
		"y", &alt::Rotation::pitch,
		"z", &alt::Rotation::yaw
	);

	lua["PlayerConnectEvent"] = std::vector<sol::function>{};
	lua["PlayerDisconnectEvent"] = std::vector<sol::function>{};

	//lua["onPlayerConnect"] = new sol::table(lua.lua_state(), sol::new_table());

	// Open lua main file and all packages in same directory.
	lua["package"]["path"] = info->path + "?.lua";

	// Run the main script.
	lua.script_file(info->path.ToString() + "\\" + info->main.ToString());
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
			reinterpret_cast<const alt::CPlayerConnectEvent*>(ev)->GetTarget();

			auto connectedPlayer = reinterpret_cast<const alt::CPlayerConnectEvent*>(ev)->GetTarget();
			std::vector<sol::function>& refToEvents = lua["PlayerConnectEvent"];

			for (const auto& func : refToEvents) {
				func(connectedPlayer);
			}
			break;
	}


	return true;
}
