#include "sdk.h"
#include "Math.h"
#include "global.h"
#include "GameUtils.h"
#include "xor.h"
CBaseCombatWeapon* CBaseEntity::GetWeapon()
{
	ULONG WeaponUlong = *(PULONG)((DWORD)this + offys.m_hActiveWeapon); // hActiveWeapon
	return (CBaseCombatWeapon*)(g_pEntitylist->GetClientEntityFromHandle(WeaponUlong));
}
DWORD GetCSWpnDataAddr;
CSWeaponInfo* CBaseCombatWeapon::GetCSWpnData()
{

	typedef CSWeaponInfo*(__thiscall* OriginalFn)(void*);
	return  getvfunc<OriginalFn>(this, 446)(this);
}
#define TIME_TO_TICKS( dt )	( ( int )( 0.5f + ( float )( dt ) / g_pGlobals->interval_per_tick ) )
int CBaseEntity::GetChockedPackets()
{
	if (GetSimulationTime() > GetOldSimulationTime())
		return TIME_TO_TICKS(fabs(GetSimulationTime() - GetOldSimulationTime()));
	return 0;
}
Vector& CBaseEntity::m_vecNetworkOrigin() 
{
	
	static int offset = g_pNetVars->GetOffset("DT_CSPlayer", "m_flFriction") - sizeof(Vector);
	return *(Vector*)((DWORD)this + offset);
}

float CBaseEntity::GetOldSimulationTime()
{

		static uintptr_t offset = g_pNetVars->GetOffset("DT_CSPlayer", "m_flSimulationTimen") +0x4;
	return *(float*)((DWORD)this + offset);
}
bool CBaseEntity::SetupBones(VMatrix *pBoneToWorldOut, int nMaxBones, int boneMask, float currentTime)
{
	//void *pRenderable = (void*)(this + 0x4);
	//typedef bool(__thiscall* OriginalFn)(PVOID, VMatrix*, int, int, float);
	//return getvfunc<OriginalFn>(pRenderable, 13)(pRenderable, pBoneToWorldOut, nMaxBones, boneMask, currentTime);

	__asm
	{
		mov edi, this
		lea ecx, dword ptr ds : [edi + 0x4]
		mov edx, dword ptr ds : [ecx]
		push currentTime
		push boneMask
		push nMaxBones
		push pBoneToWorldOut
		call dword ptr ds : [edx + 0x34]
	}
}

bool CBaseEntity::IsTargettingLocal()
{

	Vector src, dst, forward;
	trace_t tr;

	if (!this || !G::LocalPlayer || G::LocalPlayer->GetHealth() < 0)
		return false;

	Vector viewangle = this->GetEyeAngles();

	Math::AngleVectors(viewangle, &forward);
	forward *= 8142.f;
	src = this->GetEyePosition();
	dst = src + forward;

	Ray_t ray;
	ray.Init(src, dst);
	CTraceCBaseEntity filter;
	filter.pHit = G::LocalPlayer;

	g_pEngineTrace->TraceRay_NEW(ray, MASK_SHOT, &filter, &tr);
	

	if (tr.m_pEnt && tr.m_pEnt->GetTeamNum() != this->GetTeamNum()/*== G::LocalPlayer*/)
		return true;

	return false;
}
bool CBaseEntity::IsPlayer()
{
	ClientClass* pClass = (ClientClass*)this->GetClientClass();
	return pClass->m_ClassID == 35;
}