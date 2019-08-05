#include "pch.h"
#include "LuaLoader.h"

LuaLoader::LuaLoader(alt::IServer* server, alt::IResource::CreationInfo* info) {
	this->server = server;

	lua.open_libraries(sol::lib::base, sol::lib::package);


	// Setup Namespace for alt
	sol::table alt = lua["alt"].get_or_create<sol::table>();

	// Bind Functions
	this->BindIPlayer();
	this->BindIServer(alt);

	alt.new_usertype<alt::StringView>("StringView",
		sol::constructors<alt::StringView(std::string)>(),
		"ToString", &alt::StringView::ToString
		);

	// Bind Vector3
	alt.new_usertype<alt::Position>("Vector3",
		sol::constructors<alt::Position(), // Default Constructor
		alt::Position(float, float, float)>(), // Constructor
		// Vector3 Properties
		"x", &alt::Position::x,
		"y", &alt::Position::y,
		"z", &alt::Position::z
	);

	// Bind Rotation
	alt.new_usertype<alt::Rotation>("Rotation",
		sol::constructors<alt::Rotation(), alt::Rotation(float, float, float)>(),
		"x", &alt::Rotation::roll,
		"y", &alt::Rotation::pitch,
		"z", &alt::Rotation::yaw
	);

	// Events to Add To
	lua["PlayerConnectEvent"] = std::vector<sol::function>{};
	lua["PlayerDisconnectEvent"] = std::vector<sol::function>{};

	// Run the main script.
	this->LoadPackages(info);
	this->LoadMainResourceFile(info);
}

// Load all lua files in directory.
void LuaLoader::LoadPackages(alt::IResource::CreationInfo* info) 
{
	lua["package"]["path"] = info->path + "?.lua";
}

// Load main resource file in directory specified by config.
void LuaLoader::LoadMainResourceFile(alt::IResource::CreationInfo* info) {
	lua.script_file(info->path.ToString() + "\\" + info->main.ToString());
}

// Bind all 'alt.X' server functionality.
void LuaLoader::BindIServer(sol::table alt) {
	alt.set_function("hash", &LuaLoader::GetHash, this);
	alt.set_function("log", &LuaLoader::LogServer, this);
	alt.set_function("logError", &LuaLoader::LogError, this);
	alt.set_function("getVehicles", &alt::IServer::GetVehicles, server);
	alt.set_function("getPlayers", sol::readonly(&LuaLoader::currentPlayers), this);
	alt.set_function("getPlayersByName", &LuaLoader::GetPlayersByName, this);
	alt.set_function("getEntityByID", &alt::IServer::GetEntityByID, server);
	alt.set_function("createVehicle", &alt::IServer::CreateVehicle, server);
	alt.set_function("createColshapeCylinder", &alt::IServer::CreateColShapeCylinder, server);
	alt.set_function("createColshapeSphere", &alt::IServer::CreateColShapeSphere, server);
	alt.set_function("createColShapeCircle", &alt::IServer::CreateColShapeCircle, server);
	alt.set_function("createColShapeCube", &alt::IServer::CreateColShapeCube, server);
	alt.set_function("createColShapeRectangle", &alt::IServer::CreateColShapeRectangle, server);
}

void LuaLoader::BindColshapes() {
	/*
	lua.new_usertype<alt::IColShape>("ColShape",
		"GetPosition", &alt::IColShape::GetPosition,
		"GetDimension", &alt::IColShape::GetDimension,
		"SetDimension", &alt::IColShape::SetDimension,
		"SetPosition", &alt::IColShape::SetPosition,
		"Set"
	);
	*/
}

// Bind the player functionality.
void LuaLoader::BindIPlayer() {
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
}

uint32_t LuaLoader::GetHash(std::string& value)
{
	return server->Hash(value);
}

void LuaLoader::LogServer(std::string& value) {
	server->LogInfo(value);
}

void LuaLoader::LogError(std::string& value)
{
	server->LogError(value);
}

std::list<alt::IPlayer*> LuaLoader::GetPlayersByName(std::string name)
{
	std::list<alt::IPlayer*> players{};

	for each (alt::IPlayer* player in currentPlayers)
	{
		std::string playerName = player->GetName().ToString();
		std::size_t found = playerName.find(name);

		if (found != std::string::npos) {
			players.push_back(player);
		}
	}

	return players;
}
