#include "sdk.h"
#include <Psapi.h>
#include "xor.h"
#include "global.h"
offsets_t offys;
uint64_t FindPatternIDA(const char* szModule, const char* szSignature)
{
	//CREDITS: learn_more
#define INRANGE(x,a,b)  (x >= a && x <= b) 
#define getBits( x )    (INRANGE((x&(~0x20)),XorStr('A'),XorStr('F')) ? ((x&(~0x20)) - XorStr('A') + 0xa) : (INRANGE(x,XorStr('0'),XorStr('9')) ? x - XorStr('0') : 0))
#define getByte( x )    (getBits(x[0]) << 4 | getBits(x[1]))

	MODULEINFO modInfo;
	GetModuleInformation(GetCurrentProcess(), GetModuleHandleA(szModule), &modInfo, sizeof(MODULEINFO));
	DWORD startAddress = (DWORD)modInfo.lpBaseOfDll;
	DWORD endAddress = startAddress + modInfo.SizeOfImage;
	const char* pat = szSignature;
	DWORD firstMatch = 0;
	for (DWORD pCur = startAddress; pCur < endAddress; pCur++) {
		if (!*pat) return firstMatch;
		if (*(PBYTE)pat == XorStr('\?') || *(BYTE*)pCur == getByte(pat)) {
			if (!firstMatch) firstMatch = pCur;
			if (!pat[2]) return firstMatch;
			if (*(PWORD)pat == XorStr('\?\?') || *(PBYTE)pat != XorStr('\?')) pat += 3;
			else pat += 2;    //one ?
		}
		else {
			pat = szSignature;
			firstMatch = 0;
		}
	}
	return NULL;
}
/*int GetOffset(char *szClassName, char *szVariable)
{
	ClientClass* pClass = g_pClient->GetAllClasses();

	for (; pClass; pClass = pClass->m_pNext)
	{
		RecvTable* pTable = pClass->m_pRecvTable;

		if (pTable->m_nProps <= 1) continue;

		for (int i = 0; i < pTable->m_nProps; i++)
		{
			RecvProp* pProp = &pTable->m_pProps[i];

			if (!pProp) continue;

			if (strstr(pTable->m_pNetTableName, szClassName) && strstr(pProp->m_pVarName, szVariable))
			{
				//Msg(XorStr("%s found at %d\n"), szVariable, (DWORD)pProp->m_Offset);
				return pProp->m_Offset;
			}
			if (pProp->m_pDataTable)
			{
				RecvTable* pTable = pProp->m_pDataTable;
				for (int i = 0; i < pTable->m_nProps; i++)
				{
					RecvProp* pProp = &pTable->m_pProps[i];

					if (!pProp) continue;

					if (strstr(pTable->m_pNetTableName, szClassName) && strstr(pProp->m_pVarName, szVariable))
					{
						Msg(XorStr("%s found at %d\n"), szVariable, (DWORD)pProp->m_Offset);
						return pProp->m_Offset;
					}
				}
			}
		}
	}
	//Msg(XorStr("Fucked up, no offset for %s\n"), szVariable);
	return 0; //fucked up town, shit fly in my mouth. I cant seeeeeeeeeeeeeeeeeeee
}*/


void SetupOffsets()
{
	printf(XorStr(":::::Adresses & Offsets:::::\n"));
	g_pNetVars = new CNetVars();

	
	offys.nWriteableBones = g_pNetVars->GetOffset(XorStr("DT_CSPlayer"), XorStr("m_nForceBone")) + 0x20;
	offys.dwOcclusionArray = *(uintptr_t*)(FindPatternIDA(XorStr("client.dll"), XorStr("A1 ? ? ? ? 8B B7 ? ? ? ? 89 75 F8")) + 0x1);
	offys.bDidCheckForOcclusion = *(uintptr_t*)(FindPatternIDA(XorStr("client.dll"), XorStr("A1 ? ? ? ? 8B B7 ? ? ? ? 89 75 F8")) + 0x7);
	offys.InvalidateBoneCache = FindPatternIDA("client.dll", "80 3D ? ? ? ? 00 74 16 A1");
	
	printf(XorStr("Array: 0x%X\n"), (DWORD)offys.dwOcclusionArray);
	printf(XorStr("Bool: 0x%X\n"), (DWORD)offys.bDidCheckForOcclusion);

offys.dwInitKeyValues = FindPatternIDA(XorStr("client.dll"), XorStr("8B 0E 33 4D FC 81 E1 ? ? ? ? 31 0E 88 46 03 C1 F8 08 66 89 46 12 8B C6"))-0x45;
printf(XorStr("InitKeyValues: 0x%X\n"), (DWORD)offys.dwInitKeyValues);

offys.dwLoadFromBuffer = FindPatternIDA(XorStr("client.dll"), XorStr("55 8B EC 83 E4 F8 83 EC 34 53 8B 5D 0C 89 4C 24 04"));
printf(XorStr("LoadFromBuffer: 0x%X\n"), (DWORD)offys.dwLoadFromBuffer);

offys.m_ArmorValue = g_pNetVars->GetOffset("DT_CSPlayer", "m_ArmorValue");
offys.m_bHasHelmet = g_pNetVars->GetOffset("DT_CSPlayer", "m_bHasHelmet");

offys.m_flPoseParameter = g_pNetVars->GetOffset(XorStr("DT_CSPlayer"), XorStr("m_flPoseParameter"));
offys.m_flCycle = g_pNetVars->GetOffset(XorStr("DT_CSPlayer"), XorStr("m_flCycle"));
offys.m_flSimulationTime = g_pNetVars->GetOffset(XorStr("DT_CSPlayer"), XorStr("m_flSimulationTime"));
offys.m_flAnimTime = g_pNetVars->GetOffset(XorStr("DT_CSPlayer"), XorStr("m_flAnimTime"));
offys.m_nSequence = g_pNetVars->GetOffset(XorStr("DT_CSPlayer"), XorStr("m_nSequence"));
offys.m_flLowerBodyYawTarget = g_pNetVars->GetOffset(XorStr("DT_CSPlayer"), XorStr("m_flLowerBodyYawTarget"));
offys.m_angEyeAngles = g_pNetVars->GetOffset(XorStr("DT_CSPlayer"), XorStr("m_angEyeAngles[0]"));
offys.m_nTickBase = g_pNetVars->GetOffset(XorStr("DT_BasePlayer"), XorStr("m_nTickBase"));
offys.m_flNextPrimaryAttack = g_pNetVars->GetOffset(XorStr("DT_BaseCombatWeapon"), XorStr("m_flNextPrimaryAttack"));
offys.m_vecOrigin = g_pNetVars->GetOffset(XorStr("DT_BaseEntity"), XorStr("m_vecOrigin"));
offys.m_vecVelocity = g_pNetVars->GetOffset("DT_CSPlayer", "m_vecVelocity[0]");
offys.m_vecBaseVelocity = g_pNetVars->GetOffset("DT_CSPlayer", "m_vecBaseVelocity");
offys.m_flFallVelocity = g_pNetVars->GetOffset("DT_CSPlayer", "m_flFallVelocity");
offys.m_bPinPulled = g_pNetVars->GetOffset(XorStr("DT_BaseCSGrenade"), XorStr("m_bPinPulled"));
offys.m_fThrowTime = g_pNetVars->GetOffset(XorStr("DT_BaseCSGrenade"), XorStr("m_fThrowTime"));
offys.m_fAccuracyPenalty = g_pNetVars->GetOffset("DT_WeaponCSBase", "m_fAccuracyPenalty");

offys.m_flC4Blow = g_pNetVars->GetOffset(XorStr("DT_PlantedC4"), XorStr("m_flC4Blow"));
offys.m_bBombDefused= g_pNetVars->GetOffset(XorStr("DT_PlantedC4"), XorStr("m_bBombDefused"));
offys.m_hOwnerEntity= g_pNetVars->GetOffset(XorStr("DT_PlantedC4"), XorStr("m_hOwnerEntity"));

offys.m_flFriction = g_pNetVars->GetOffset("DT_CSPlayer", "m_flFriction");
offys.m_CollisionGroup = g_pNetVars->GetOffset("DT_BaseEntity", "m_CollisionGroup");

uintptr_t player_resource_pointer= uintptr_t((uintptr_t)GetModuleHandle("client.dll") + 0x2EC5ADC);
offys.dw_CSPlayerResource = (DWORD)player_resource_pointer;
offys.m_iCompetetiveRanking = g_pNetVars->GetOffset(XorStr("DT_CSPlayerResource"), XorStr("m_iCompetitiveRanking"));
offys.m_iCompetetiveWins = g_pNetVars->GetOffset(XorStr("DT_CSPlayerResource"), XorStr("m_iCompetitiveWins"));
offys.m_iPing = g_pNetVars->GetOffset(XorStr("DT_CSPlayerResource"), XorStr("m_iPing"));

offys.m_hMyWearables = g_pNetVars->GetOffset(XorStr("DT_CSPlayer"), XorStr("m_hMyWearables"));

offys.m_hMyWeapons = g_pNetVars->GetOffset(XorStr("DT_BasePlayer"), XorStr("m_hMyWeapons"));
offys.m_hActiveWeapon = g_pNetVars->GetOffset(XorStr("DT_BasePlayer"), XorStr("m_hActiveWeapon"));

offys.m_flFriction = g_pNetVars->GetOffset("DT_CSPlayer", "m_flFriction");
offys.m_flMaxspeed = g_pNetVars->GetOffset("DT_BasePlayer", "m_flMaxspeed");
offys.m_flStepSize = g_pNetVars->GetOffset("DT_CSPlayer", "m_flStepSize");
offys.m_bGunGameImmunity = g_pNetVars->GetOffset("DT_CSPlayer", "m_bGunGameImmunity");

offys.m_flPostponeFireReadyTime = g_pNetVars->GetOffset("DT_WeaponCSBaseGun", "m_flPostponeFireReadyTime");
offys.m_iShotsFired = g_pNetVars->GetOffset("DT_CSPlayer", "m_iShotsFired");

/*DT_BaseAttributableItem*/
offys.m_iItemDefinitionIndex = g_pNetVars->GetOffset("DT_BaseAttributableItem", "m_iItemDefinitionIndex");
offys.m_iItemIDHigh = g_pNetVars->GetOffset("DT_BaseAttributableItem", "m_iItemIDHigh");
offys.m_iItemIDLow= g_pNetVars->GetOffset("DT_BaseAttributableItem", "m_iItemIDLow");
offys.m_iEntityQuality = g_pNetVars->GetOffset("DT_BaseAttributableItem", "m_iEntityQuality");
offys.m_szCustomName = g_pNetVars->GetOffset("DT_BaseAttributableItem", "m_szCustomName");
offys.m_OriginalOwnerXuidLow = g_pNetVars->GetOffset("DT_BaseAttributableItem", "m_OriginalOwnerXuidLow");
offys.m_OriginalOwnerXuidHigh = g_pNetVars->GetOffset("DT_BaseAttributableItem", "m_OriginalOwnerXuidHigh");
offys.m_nFallbackPaintKit = g_pNetVars->GetOffset("DT_BaseAttributableItem", "m_nFallbackPaintKit");
offys.m_nFallbackSeed = g_pNetVars->GetOffset("DT_BaseAttributableItem", "m_nFallbackSeed");
offys.m_flFallbackWear = g_pNetVars->GetOffset("DT_BaseAttributableItem", "m_flFallbackWear");
offys.m_nFallbackStatTrak = g_pNetVars->GetOffset("DT_BaseAttributableItem", "m_nFallbackStatTrak");
offys.m_nAccountID = g_pNetVars->GetOffset("DT_BaseAttributableItem", "m_nAccountID");

offys.m_nModeIndex = g_pNetVars->GetOffset("DT_BaseViewModel", "m_nModelIndex");
offys.m_hViewModel = g_pNetVars->GetOffset("DT_BasePlayer", "m_hViewModel[0]");
if (!offys.m_hViewModel)
offys.m_hViewModel = 0x32FC;
offys.m_hWeapon = g_pNetVars->GetOffset("DT_BaseViewModel", "m_hWeapon");
}