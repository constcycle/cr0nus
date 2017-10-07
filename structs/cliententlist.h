class CClientEntityList
{
public:

	CBaseEntity* GetClientEntity( int Index )
	{
		typedef CBaseEntity*(__thiscall* Fn)(void*, int);
		return getvfunc<Fn>(this, 3)(this, Index);
	}

	int GetHighestEntityIndex()
	{
		typedef int(__thiscall* Fn)(void*);
		return getvfunc<Fn>(this, 8)(this);
	}

	CBaseEntity* GetClientEntityFromHandle( HANDLE Handle )
	{
		typedef CBaseEntity*(__thiscall* Fn)(void*,HANDLE);
		return getvfunc<Fn>(this, 7)(this, Handle);
	}
	CBaseEntity *	GetClientEntityFromHandle(ULONG hEnt)
	{
		typedef CBaseEntity*(__thiscall* Fn)(void*, ULONG);
		return getvfunc<Fn>(this, 4)(this, hEnt);
	}
	CBaseEntity *	GetClientEntityFromHandle_D(DWORD hEnt)
	{
		typedef CBaseEntity*(__thiscall* Fn)(void*, int);
		return getvfunc<Fn>(this, 4)(this, hEnt);
	}

};