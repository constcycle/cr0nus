enum FlowType_t
{
	FLOW_OUTGOING = 0,
	FLOW_INCOMING = 1,
	FLOW_MAX = 2
};
enum Typetype_t
{
	TYPE_GENERIC = 0,
	TYPE_LOCALPLAYER = 1
};
class NET_SetConVar
{
public:
	NET_SetConVar(const char* name, const char* value);
	~NET_SetConVar();
private:
	DWORD pad[13];
};
class INetChannelInfo
{
public:
	float GetLatency(int flow)
	{
		typedef float(__thiscall* Fn)(void*, int);
		 return getvfunc<Fn>(this, 9)(this, flow);
	}
	float GetAvgLatency(int type)
	{
		typedef float(__thiscall* Fn)(void*, int);
		return getvfunc<Fn>(this, 10)(this, type);
	}
	void SendNetMsg(NET_SetConVar& convar)
	{
		typedef void(__thiscall* oSendNetMsg)(PVOID, NET_SetConVar&, int, int);
		getvfunc<oSendNetMsg>(this, 42)(this, convar, 0, 0);
	}
};
class CEngine
{
public:
	void SetName(std::string name);
	INetChannelInfo* GetNetChannelInfo()
	{
		typedef INetChannelInfo* (__thiscall* Fn)(void*);
		return getvfunc<Fn>(this, 78)(this);
	}
	void GetScreenSize( int& Width, int& Height )
	{
		typedef void( __thiscall* Fn )(void*, int&, int&);
		return getvfunc<Fn>(this, 5)(this, Width, Height);
	}

	bool GetPlayerInfo( int Index, player_info_t* PlayerInfo )	
	{
		typedef bool(__thiscall* Fn)(void*, int, player_info_t*);
		return getvfunc<Fn>(this, 8)(this, Index, PlayerInfo);
	}

	int GetLocalPlayer()
	{
		typedef int(__thiscall* Fn)(void*);
		return getvfunc<Fn>(this, 12)(this);
	}
	int GetPlayerForUserID(int userid)
	{
		typedef int(__thiscall* Fn)(void*, int);
		return getvfunc<Fn>(this, 9)(this, userid);
	}
	void ClientCmd( const char* Command )
	{
		typedef void(__thiscall* Fn)(void*, const char*);
		return getvfunc<Fn>(this, 108)(this, Command);
		}

	void SetViewAngles(QAngle& Angles)
	{
		typedef void(__thiscall* Fn)(void*, QAngle&);
		return getvfunc<Fn>(this, 19)(this, Angles);
	}

	void GetViewAngles(QAngle& angle)
	{
		typedef void(__thiscall* Fn)(void*, QAngle&);
		return getvfunc<Fn>(this, 18)(this,angle);
	}

	matrix3x4_t& WorldToScreenMatrix()
	{
		typedef matrix3x4_t& (__thiscall* Fn)(void*);
		return getvfunc<Fn>(this, 37)(this);
		
	
	}
	bool IsConnected()
	{
		typedef bool(__thiscall* Fn)(PVOID);
		return getvfunc<Fn>(this, 27)(this);
	}

	bool IsInGame()
	{
		typedef bool(__thiscall* Fn)(PVOID);
		return getvfunc<Fn>(this, 26)(this);
	}

	int GetMaxClients()
	{
		typedef bool(__thiscall* Fn)(PVOID);
		return getvfunc<Fn>(this, 20)(this);
	}

	float Time()
	{
		typedef float(__thiscall* Fn)(PVOID);
		return getvfunc<Fn>(this, 14)(this);
	}
	void ClientCmd_Unrestricted(char  const* cmd, char flag)
	{
		typedef void(__thiscall* oClientCmdUnres)(PVOID, const char*, char);
		return getvfunc<oClientCmdUnres>(this, 114)(this, cmd, flag);
	}
	//52
	char const    GetLevelName()
	{
		typedef char const(__thiscall* oClientCmdUnres)(PVOID);
		return getvfunc<oClientCmdUnres>(this, 52)(this);
	}
	unsigned int GetEngineBuildNumber()
	{
		typedef unsigned int(__thiscall* OriginalFn)(PVOID);
		return getvfunc< OriginalFn >(this, 104)(this);
	}

};