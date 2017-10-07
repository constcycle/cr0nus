#include "hooks.h"
#include <time.h>
#include "Mmsystem.h"
#include <thread>
#include "Hitmarker.h"
#include "Resolver.h"
#include "global.h"

#include "controls.h"
#pragma comment(lib, "winmm.lib") 
#define EVENT_HOOK( x )
#define TICK_INTERVAL			(Globals->interval_per_tick)


#define TIME_TO_TICKS( dt )		( (int)( 0.5f + (float)(dt) / TICK_INTERVAL ) )
cGameEvent g_Event;


int KillsPerRound;
int GetServerSideID(int CBaseEntityIndex)
{
	player_info_t pInfo;
	g_pEngine->GetPlayerInfo(CBaseEntityIndex, &pInfo);
	return pInfo.m_nUserID;
}
std::map<int, ResolveData> PlayerData;
void cGameEvent::FireGameEvent(IGameEvent *event)
{
	/*short   m_nUserID		user ID who was hurt
	short	attacker	user ID who attacked
	byte	health		remaining health points
	byte	armor		remaining armor points
	string	weapon		weapon name attacker used, if not the world
	short	dmg_health	damage done to health
	byte	dmg_armor	damage done to armor
	byte	hitgroup	hitgroup that was damaged*/
	const char* szEventName = event->GetName();
	if (!szEventName)	
		return;


	if (strcmp(szEventName, "round_end") == 0)
	{
		if (global_damagelist->damage_infos.size() > 0)
			global_damagelist->damage_infos.clear();
	}
	if (strcmp(szEventName, "round_start") == 0)
	{

	}
	if (strcmp(szEventName, "weapon_fire") == 0)
	{

	}
	if (strcmp(szEventName, "player_hurt") == 0)
	{
		bool IsHeadshot = (event->GetInt(XorStr("hitgroup")) == 1);

		int damage = event->GetInt("dmg_health");

		int CBaseEntityServerID =event->GetInt(XorStr("userID"));
		int AttackerServerID = event->GetInt(XorStr("attacker"));
		int AttackerIndex = g_pEngine->GetPlayerForUserID(AttackerServerID);
		if (AttackerIndex != g_pEngine->GetLocalPlayer())
			return;
		int CBaseEntityIndex = g_pEngine->GetPlayerForUserID(CBaseEntityServerID);
		if (CBaseEntityIndex == g_pEngine->GetLocalPlayer())
			return;

		g_Hitmarker->update_end_time();
		g_Hitmarker->play_sound();
		
		player_info_s pl_info;
		g_pEngine->GetPlayerInfo(CBaseEntityIndex, &pl_info);

		if (global_damagelist->damage_infos.size() > 21)
			global_damagelist->damage_infos.erase(global_damagelist->damage_infos.begin());

		std::string dmg_info = ""; dmg_info.append(pl_info.m_szPlayerName); dmg_info.append(" was damaged "); dmg_info.append(std::to_string(damage)); dmg_info.append(" HP!");
			global_damagelist->damage_infos.push_back(dmg_info);
		
		{
		
			int hitgroup = event->GetInt("hitgroup");
			if (hitgroup == 4)
			{
				PlayerData[CBaseEntityServerID].LastAttemptedAngles.y -= 45;
				//PlayerData[CBaseEntityServerID].LastAnglesHit.y = PlayerData[CBaseEntityServerID].LastAttemptedAngles.y;
				PlayerData[CBaseEntityServerID].MissedShots = 0;
			}
			else if (hitgroup == 5)
			{
				PlayerData[CBaseEntityServerID].LastAttemptedAngles.y += 45;
				//PlayerData[CBaseEntityServerID].LastAnglesHit.y = PlayerData[CBaseEntityServerID].LastAttemptedAngles.y;
				PlayerData[CBaseEntityServerID].MissedShots = 0;
			}
			else if (hitgroup == 2)
			{
				PlayerData[CBaseEntityServerID].MissedShots = 0;
			}
			
			Msg("[Hit registered %i]\n", hitgroup);
		}
	}
	if (strcmp(szEventName, "player_death") == 0)
	{

		
		int CBaseEntityServerID = event->GetInt(XorStr("userID"));
		int AttackerServerID = event->GetInt(XorStr("attacker"));
		int AttackerIndex = g_pEngine->GetPlayerForUserID(AttackerServerID);
		if (AttackerIndex != g_pEngine->GetLocalPlayer())
			return;
		int CBaseEntityIndex = g_pEngine->GetPlayerForUserID(CBaseEntityServerID);
		if (CBaseEntityIndex == g_pEngine->GetLocalPlayer())
			return;
			
		G::resolver_ticks[CBaseEntityIndex] = 0;

			/*if (std::string(event->GetString("weapon")).find("knife_") != std::string::npos)
			{
				std::string knifebuffer;
				auto KnifeType = GlobalSkinchanger->Skinchanger_Knife->SelectedIndex;
				if (KnifeType > 0)
				{
					if (KnifeType == 1)
						knifebuffer = "knife_karambit";
					if (KnifeType == 2)
						knifebuffer = "bayonet";
					if (KnifeType == 3)
						knifebuffer = "knife_m9_bayonet";
					if (KnifeType == 4)
						knifebuffer = "knife_tactical";
					if (KnifeType == 5)
						knifebuffer = "knife_gut";
					if (KnifeType == 6)
						knifebuffer = "knife_falchion";
					if (KnifeType == 7)
						knifebuffer = "knife_push";
					if (KnifeType == 8)
						knifebuffer = "knife_butterfly";
					if (KnifeType == 9)
						knifebuffer = "knife_flip";
					if (KnifeType == 10)
						knifebuffer = "knife_bowie";
					event->SetString("weapon", knifebuffer.c_str());
				}
			}*/
	}
}

int cGameEvent::GetEventDebugID()
{
	return 42;
}

void cGameEvent::RegisterSelf()
{

	//g_EventManager2->AddListener( this, "round_start", false );
	g_pGameEventManager->AddListener(this, "player_connect", false);
	g_pGameEventManager->AddListener(this, "player_hurt", false);
	g_pGameEventManager->AddListener(this, "round_start", false);
	g_pGameEventManager->AddListener(this, "round_end", false);
	g_pGameEventManager->AddListener(this, "player_death", false);
	//g_pGameEventManager->AddListener(this, "weapon_fire", false);
}

void cGameEvent::Register()
{
	EVENT_HOOK(FireEvent);
}