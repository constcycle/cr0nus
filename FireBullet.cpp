#include "hooks.h"
#include "Math.h"
#include "BacktrackingHelper.h"
#include "Resolver.h"
void __fastcall/*__stdcall*/ Hooks::FireBullets_PostDataUpdate(C_TEFireBullets* thisptr, DataUpdateType_t updateType)
{
	int index = thisptr->m_iPlayer + 1;
	player_info_s pInfo;
	CBaseEntity* pCBaseEntity = g_pEntitylist->GetClientEntity(index);
	if (pCBaseEntity && pCBaseEntity->GetHealth() > 0 && g_pEngine->GetPlayerInfo(thisptr->m_iPlayer + 1, &pInfo))
	{
		float yaw = Math::NormalizeYaw(thisptr->m_vecAngles.y);//normalize
		printf("Event: Index: %i Name: %s   |||||Shot YAW: %f |||||| Normal Yaw: %f  \n", thisptr->m_iPlayer + 1, pInfo.m_szPlayerName, yaw, g_BacktrackHelper->PlayerRecord[index].EyeAngles.y);
		//g_Resolver->StoreVars(pCBaseEntity, Vector(thisptr->m_vecAngles.x, g_BacktrackHelper->PlayerRecord[index].EyeAngles.y), yaw);
	}
	firebulletVMT->GetOriginalMethod<TE_FireBullets_PostDataUpdateFn>(7)(thisptr, updateType);	
}