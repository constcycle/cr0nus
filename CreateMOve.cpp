#include "hooks.h"
#include "global.h"
#include "Misc.h"
#include "Menu.h"
#include "BacktrackingHelper.h"
#include "Math.h"
#include "GameUtils.h"
#include "Aimbot.h"
#include "PredictionSystem.h"
#include "Antiaim.h"
#include "Resolver.h"
void __declspec(naked) LagFix()
{
	__asm
	{
		push edi
		xchg dword ptr[esp], edi
		push eax
		mov eax, 77
		mov eax, dword ptr[esp]
		add esp, 4
		pop edi

		cmp esp, 0
		jne fixentity

		_emit 0x88
		_emit 0xFF

		invlpg dword ptr[eax]

		int 2

		fixentity:
		sub esp, 4
			mov dword ptr[esp], ebp

			call cleanup

			pop ebp

			ret

			cleanup :

		ret
	}
}
bool __fastcall Hooks::CreateMove(void* thisptr, void*, float flInputSampleTime, CUserCmd* cmd)
{
	//clientmodeVMT->GetOriginalMethod<CreateMoveFn>(24)(thisptr, flInputSampleTime, cmd);
	if (cmd)
	{
		
		if (!cmd->command_number)
			return true;

		
			g_pEngine->SetViewAngles(cmd->viewangles);
			QAngle org_view = cmd->viewangles;

			CBaseEntity* pLocalPlayer  = g_pEntitylist->GetClientEntity(g_pEngine->GetLocalPlayer());
			if (pLocalPlayer)
			{
				G::LocalPlayer = pLocalPlayer;
				G::UserCmd = cmd;
				G::UserCmdForBacktracking = cmd;

				for (int i = 1; i < g_pGlobals->maxClients; i++)
				{
					if (i == g_pEngine->GetLocalPlayer())
						continue;
					CBaseEntity* pEntity = g_pEntitylist->GetClientEntity(i);
					if (pEntity)
					{
						if (pEntity->GetHealth() > 0 && !pEntity->IsDormant())
						{
							g_Resolver->StoreVars(pEntity);
						}
					}
				}

				if (pLocalPlayer->GetHealth() > 0)
				{


					CBaseCombatWeapon* pWeapon = pLocalPlayer->GetWeapon();
					if (pWeapon)
					{


						PVOID pebp;
						__asm mov pebp, ebp;
						bool* pbSendPacket = (bool*)(*(PDWORD)pebp - 0x1C);
						bool& bSendPacket = *pbSendPacket;

						

						G::MainWeapon = pWeapon;		

						G::WeaponData =  pWeapon->GetCSWpnData();

						bool IsLadder = pLocalPlayer->GetMoveType() == MOVETYPE_LADDER;
						G::StrafeAngle = G::UserCmd->viewangles;

						g_Aimbot->DropTarget();
						//g_Misc->RankReveal();

						g_Misc->Bunnyhop();
						g_Misc->AutoStrafe();


						if ( (G::UserCmd->buttons & IN_ATTACK || 
							(G::UserCmd->buttons & IN_ATTACK2 && (G::MainWeapon->WeaponID() == REVOLVER || G::MainWeapon->IsKnife()))) && GameUtils::IsAbleToShoot())
							g_Aimbot->fired_in_that_tick = true;

						g_PredictionSystem->StartPrediction();

					
						//LagFix();
						g_Aimbot->Run();

						if(!g_Aimbot->aimbotted_in_current_tick)
							g_Aimbot->AutoRevolver();

						g_Aimbot->CompensateInaccuracies();

						g_Misc->FakeLag();

						G::ForceRealAA = false;
						if (G::ChokedPackets >= 14)
						{
							G::SendPacket = true;
							G::ChokedPackets = 0;
							G::ForceRealAA = true;
						}

						if (!IsLadder)
							g_Antiaim->Run(org_view);

						g_PredictionSystem->EndPrediction();

						if ((G::UserCmd->buttons & IN_ATTACK || G::UserCmd->buttons & IN_ATTACK2 && G::MainWeapon->WeaponID() == REVOLVER) && (G::MainWeapon->IsPistol() || G::MainWeapon->WeaponID() == AWP || G::MainWeapon->WeaponID() == SSG08))
						{
							static bool bFlip = false;
							if (bFlip)
							{
								if (G::MainWeapon->WeaponID() == REVOLVER)
								{
								}
								else
									G::UserCmd->buttons &= ~IN_ATTACK;
							}
							bFlip = !bFlip;
						}



						if (G::SendPacket)
						{
							if(MiscElements.Misc_General_Thirdperson_Angles->SelectedIndex == 0)
								G::AAAngle = G::UserCmd->viewangles;

							G::ChokedPackets = 0;
							G::FakeAngle = G::UserCmd->viewangles;
						}
						else
						{
							if (MiscElements.Misc_General_Thirdperson_Angles->SelectedIndex == 1 && !g_Aimbot->aimbotted_in_current_tick)
								G::AAAngle = G::UserCmd->viewangles;

							G::ChokedPackets++;
							G::RealAngle = G::UserCmd->viewangles;
						}
						//lby angles
						if (MiscElements.Misc_General_Thirdperson_Angles->SelectedIndex == 2)
							G::AAAngle = QAngle(G::UserCmd->viewangles.x, G::LocalPlayer->LowerBodyYaw());

						if ((G::UserCmd->buttons & IN_ATTACK || G::UserCmd->buttons & IN_ATTACK2 && G::MainWeapon->WeaponID() == REVOLVER) && GameUtils::IsAbleToShoot() && g_Aimbot->target_index != -1 && !(g_Aimbot->pTarget->GetFlags() & FL_ONGROUND))
						{
							G::resolver_ticks[g_Aimbot->target_index]++;
							int ID = GetServerSideID(g_Aimbot->target_index);
							PlayerData[ID].MissedShots++;
						//	g_Resolver->resolve_infos[g_Aimbot->target_index].stage++;
							//if (g_Resolver->resolve_infos[g_Aimbot->target_index].stage > 5)
							//	g_Resolver->resolve_infos[g_Aimbot->target_index].stage = 0;
						}



						g_Misc->FixMovement();


						g_Misc->FixCmd(); //normalizes viewangles and clamps the movement values like upmove if a restriction is set

						cmd = G::UserCmd;
						bSendPacket = G::SendPacket;				
				}
			}
		}
	}
	return false;
}
