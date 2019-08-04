#pragma once
#define ALT_SERVER_API
#include <SDK.h>

#include <sol.hpp>
#ifdef _WIN32
#pragma comment(lib, "liblua53.a")
#endif

class LuaResource : public alt::IResource
{
	private:
		alt::IServer* server;

	public:
		LuaResource(alt::IServer* server, alt::IResource::CreationInfo* info);

		~LuaResource() override;

		void MakeClient(CreationInfo* info, alt::Array<alt::String> files) override;

		void SetExport(const char* key, const alt::MValue& mValue) {
			this->exports[key] = mValue;
		}

		bool OnEvent(const alt::CEvent* ev) override;
};