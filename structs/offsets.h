#pragma once

#include <windows.h>
#define GetNetVarOffset(x, y){return g_pNetVars->GetOffset(x, y)}
struct offsets_t
{
public:
	
	


	DWORD m_fThrowTime;
	DWORD m_bPinPulled;

	DWORD m_flC4Blow;
	DWORD m_bBombDefused;
	DWORD m_hOwnerEntity;

	DWORD m_flPoseParameter;
	DWORD m_flCycle;
	DWORD m_nSequence;
	DWORD m_flSimulationTime;
	DWORD m_flAnimTime;
	DWORD m_flLowerBodyYawTarget;

	DWORD m_vecBaseVelocity;
	DWORD m_vecVelocity;
	DWORD m_flFallVelocity;
	DWORD m_flMaxspeed;
	DWORD m_flStepSize;

	DWORD m_bHasHelmet;
	DWORD m_ArmorValue;

	DWORD nWriteableBones;
	DWORD dwOcclusionArray;
	DWORD bDidCheckForOcclusion;
	DWORD InvalidateBoneCache;

	DWORD m_fAccuracyPenalty;

	DWORD m_CollisionGroup;

	DWORD dw_CSPlayerResource;
	DWORD m_iCompetetiveRanking;
	DWORD m_iCompetetiveWins;
	DWORD m_iKills;
	DWORD m_iAssists;
	DWORD m_iDeaths;
	DWORD m_iPing;
	DWORD m_iScore;
	DWORD m_szClan;

	DWORD m_vecOrigin;
	DWORD m_flNextPrimaryAttack;
	DWORD m_nTickBase;

	DWORD m_angEyeAngles;

	DWORD m_hMyWearables;

	DWORD m_flFriction;

	DWORD m_hMyWeapons;
	DWORD m_hActiveWeapon;

	DWORD m_flMaxSpeed;

	DWORD m_bGunGameImmunity;

	DWORD m_flPostponeFireReadyTime;

	DWORD m_iShotsFired;

	/*Skinchanger*/
	
		DWORD m_iItemDefinitionIndex;
		DWORD m_iItemIDHigh;
		DWORD m_iItemIDLow;
		DWORD m_iEntityQuality;
		DWORD m_szCustomName;
		DWORD m_OriginalOwnerXuidLow;
		DWORD m_OriginalOwnerXuidHigh;
		DWORD m_nFallbackPaintKit;
		DWORD m_nFallbackSeed;
		DWORD m_flFallbackWear;
		DWORD m_nFallbackStatTrak;
		DWORD m_nAccountID;

		DWORD m_nModeIndex;
		DWORD m_hViewModel;
		DWORD m_hWeapon;

DWORD dwLoadFromBuffer;
DWORD dwInitKeyValues;
};
extern void SetupOffsets();
extern offsets_t offys;
extern uint64_t FindPatternIDA(const char* szModule, const char* szSignature);