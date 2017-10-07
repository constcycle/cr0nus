#pragma once
class IMoveHelper
{
public:
	void SetHost(CBaseEntity *pPlayer)
	{
		typedef void(__thiscall* OriginalFn)(void*, CBaseEntity *pPlayer);
		getvfunc<OriginalFn>(this, 1)(this, pPlayer);
	}
};
class CPrediction
{
public:
	void SetupMove(CBaseEntity* ent, CUserCmd* cmd, void* move, void* movedata)
	{
		typedef void(__thiscall* fn)(void*, CBaseEntity*, CUserCmd*, void*, void*);
		getvfunc<fn>(this, 20)(this, ent, cmd, move, movedata);
	}

	void FinishMove(CBaseEntity* ent, CUserCmd* cmd, void* movedata)
	{
		typedef void(__thiscall* fn)(void*, CBaseEntity*, CUserCmd*, void*);
		getvfunc<fn>(this, 21)(this, ent, cmd, movedata);
	}

	void RunCommand(CBaseEntity* pEntity, CUserCmd* pCmd, void* moveHelper)
	{
		typedef void(__thiscall* fn)(void*, CBaseEntity*, CUserCmd*, void*);
		getvfunc<fn>(this, 19)(this, pEntity, pCmd, moveHelper);
	}
};

class CGameMovement
{
public:
	void ProcessMovement(CBaseEntity* ent, void* movedata)
	{
		typedef void(__thiscall* fn)(void*, CBaseEntity*, void*);
		getvfunc<fn>(this, 1)(this, ent, movedata);
	}

	void StartTrackPredictionErrors(CBaseEntity* ent)
	{
		typedef void(__thiscall* fn)(void*, CBaseEntity*);
		getvfunc<fn>(this, 3)(this, ent);
	}

	void FinishTrackPredictionErrors(CBaseEntity* ent)
	{
		typedef void(__thiscall* fn)(void*, CBaseEntity*);
		getvfunc<fn>(this, 4)(this, ent);
	}

	void DecayPunchAngle()
	{
		typedef void(__thiscall* fn)(void*);
		getvfunc<fn>(this, 19)(this);
	}
};