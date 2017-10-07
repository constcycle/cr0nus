#pragma once
class CScreen
{
public:
	int width, height;
};
class CGlobalPointers
{
public:
	CBaseEntity* LocalPlayer;
	CBaseCombatWeapon* MainWeapon;
	CUserCmd*	UserCmd;
}; extern CGlobalPointers* g_GlobalPointers;
namespace G // Global Stuff
{
	extern bool			Aimbotting;
	extern bool ForceRealAA;
	extern QAngle		AAAngle;
	extern QAngle		RealAngle;
	extern QAngle		FakeAngle;
	extern QAngle		LastAngle;
	extern QAngle StrafeAngle;
	extern bool			Return;
	extern bool InThirdperson;
	extern bool SendPacket;
	extern int ChokedPackets;
	extern int resolver_ticks[64];
	extern CScreen Screen;
	extern CBaseEntity* LocalPlayer;
	extern CBaseCombatWeapon* MainWeapon;
	extern CUserCmd*	UserCmd;
	extern CUserCmd* UserCmdForBacktracking;
	extern CSWeaponInfo* WeaponData;
	extern float lby_update_end_time;
}

extern CBaseEntity* G::LocalPlayer;
extern CBaseCombatWeapon* G::MainWeapon;
extern CUserCmd*	G::UserCmd;
