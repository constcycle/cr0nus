#include "hooks.h"
#include "global.h"
#include "Menu.h"
#include "BacktrackingHelper.h"
#include "Resolver.h"
#include "xor.h"
#include "skinchanger.h"
#define INVALID_EHANDLE_INDEX 0xFFFFFFFF

ConVar* mp_facefronttime;
ConVar* r_DrawSpecificStaticProp;

int get_model_index(int item_def_index)
{
	int ret = 0;
	switch (item_def_index)
	{
	case KNIFE:
		ret = g_pModelInfo->GetModelIndex("models/weapons/v_knife_default_ct.mdl");
		break;
	case KNIFE_T:
		ret = g_pModelInfo->GetModelIndex("models/weapons/v_knife_default_t.mdl");
		break;
	case KNIFE_KARAMBIT:
		ret = g_pModelInfo->GetModelIndex("models/weapons/v_knife_karam.mdl");
		break;
	case KNIFE_BAYONET:
		ret = g_pModelInfo->GetModelIndex("models/weapons/v_knife_bayonet.mdl");
		break;
	case KNIFE_M9_BAYONET:
		ret = g_pModelInfo->GetModelIndex("models/weapons/v_knife_m9_bay.mdl");
		break;
	case KNIFE_TACTICAL:
		ret = g_pModelInfo->GetModelIndex("models/weapons/v_knife_tactical.mdl");
		break;
	case KNIFE_GUT:
		ret = g_pModelInfo->GetModelIndex("models/weapons/v_knife_gut.mdl");
		break;
	case KNIFE_FALCHION:
		ret = g_pModelInfo->GetModelIndex("models/weapons/v_knife_falchion_advanced.mdl");
		break;
	case KNIFE_PUSH:
		ret = g_pModelInfo->GetModelIndex("models/weapons/v_knife_push.mdl");
		break;
	case KNIFE_BUTTERFLY:
		ret = g_pModelInfo->GetModelIndex("models/weapons/v_knife_butterfly.mdl");
		break;
	case KNIFE_FLIP:
		ret = g_pModelInfo->GetModelIndex("models/weapons/v_knife_flip.mdl");
		break;
	case KNIFE_BOWIE:
		ret = g_pModelInfo->GetModelIndex("models/weapons/v_knife_survival_bowie.mdl");
		break;
	default:
		break;
	}
	return ret;
}

void __stdcall Hooks::FrameStageNotify(ClientFrameStage_t curStage)
{
	static std::string old_Skyname = "";
	static bool OldNightmode;
	static int OldSky;

		if (!G::LocalPlayer || !g_pEngine->IsConnected() || !g_pEngine->IsInGame())
		{
			clientVMT->GetOriginalMethod<FrameStageNotifyFn>(36)(curStage);
			old_Skyname = "";
			OldNightmode = false;
			OldSky = 0;
			return;
		}

		
		
		if (OldNightmode != VisualElements.Visual_Misc_Nightmode->Checked)
		{
			
				if (!r_DrawSpecificStaticProp)
					r_DrawSpecificStaticProp = g_pCvar->FindVar("r_DrawSpecificStaticProp");

				r_DrawSpecificStaticProp->SetValue(0);

			for (MaterialHandle_t i = g_pMaterialSystem->FirstMaterial(); i != g_pMaterialSystem->InvalidMaterial(); i = g_pMaterialSystem->NextMaterial(i))
			{
				IMaterial* pMaterial = g_pMaterialSystem->GetMaterial(i);

				if (!pMaterial)
					continue;

				if (strstr(pMaterial->GetTextureGroupName(), "World") || strstr(pMaterial->GetTextureGroupName(), "StaticProp"))
				{
					if (VisualElements.Visual_Misc_Nightmode->Checked)
						if(strstr(pMaterial->GetTextureGroupName(), "StaticProp"))
							pMaterial->ColorModulate(0.3f, 0.3f, 0.3f);
						else
						pMaterial->ColorModulate(0.05f, 0.05f, 0.05f);
					else
						pMaterial->ColorModulate(1.0f, 1.0f, 1.0f);
				}
				
			}
			OldNightmode = VisualElements.Visual_Misc_Nightmode->Checked;
		}
		
		if (OldSky != VisualElements.Visual_Misc_Skybox->SelectedIndex)
		{
			auto LoadNamedSky = reinterpret_cast< void(__fastcall*)(const char*) >(FindPatternIDA("engine.dll", "55 8B EC 81 EC ? ? ? ? 56 57 8B F9 C7 45"));
			if (old_Skyname == "")
			{
				//old_Skyname = g_pCvar->FindVar("sv_skyname")->GetName();
			}

			int type = VisualElements.Visual_Misc_Skybox->SelectedIndex;

			/*if(type == 0)
				LoadNamedSky(old_Skyname.c_str());
			else */if(type == 1)
				LoadNamedSky("cs_baggage_skybox_");
			else if (type ==2)
				LoadNamedSky("cs_tibet");
			else if (type ==3)
				LoadNamedSky("italy");
			else if (type == 4)
				LoadNamedSky("jungle");
			else if (type == 5)
				LoadNamedSky("office");
			else if (type == 6)
				LoadNamedSky("sky_cs15_daylight02_hdr");
			else if (type == 7)
				LoadNamedSky("sky_csgo_night02");
			else if (type == 8)
				LoadNamedSky("vertigo");

			OldSky = VisualElements.Visual_Misc_Skybox->SelectedIndex;
		}



		static Vector oldViewPunch;
		static Vector oldAimPunch;


		Vector* view_punch = G::LocalPlayer->GetViewPunchPtr();
		Vector* aim_punch = G::LocalPlayer->GetPunchAnglePtr();

		auto b_IsThirdPerson = MiscElements.Misc_General_Thirdperson->Checked;//*reinterpret_cast<bool*>(reinterpret_cast<DWORD>(g_pInput) + 0xA5);//A5
		G::InThirdperson = b_IsThirdPerson;

		if (curStage == FRAME_RENDER_START && G::LocalPlayer->GetHealth() > 0)
		{
			if (b_IsThirdPerson && MiscElements.Misc_General_Thirdperson->Checked)
			{
				*reinterpret_cast<QAngle*>(reinterpret_cast<DWORD>(G::LocalPlayer + 0x31C0 + 0x8)) = G::AAAngle; //to visualize real/faked aa angles
			}
			else 
			{
				if (view_punch && aim_punch && VisualElements.Visual_Misc_NoRecoil->Checked)
				{
					oldViewPunch = *view_punch;
					oldAimPunch = *aim_punch;

					view_punch->Init();
					aim_punch->Init();
				}
			}
		}

		
	

		if (curStage == FRAME_NET_UPDATE_START)
		{
			
			for (int i = 1; i < g_pGlobals->maxClients; i++)
			{

				CBaseEntity* pEntity = g_pEntitylist->GetClientEntity(i);
				if (pEntity)
				{
					if (pEntity->GetHealth() > 0)
					{
						if (i != g_pEngine->GetLocalPlayer())
						{
							VarMapping_t* map = pEntity->GetVarMap();
							if (map)
							{
								if (RagebotElements.Ragebot_Corrections_PositionAdjustment->Checked)
								{
									map->m_nInterpolatedEntries = 0;
								}
								else
								{
									if (map->m_nInterpolatedEntries == 0)
										map->m_nInterpolatedEntries = 6;
								}
							}

						}
					}
				}
			}
		}

		

		

		if (curStage == FRAME_RENDER_START)
		{
			for (int i = 1; i < g_pGlobals->maxClients; i++)
			{
				if (i == g_pEngine->GetLocalPlayer())
					continue;
				CBaseEntity* pEntity = g_pEntitylist->GetClientEntity(i);
				if (pEntity)
				{
					if (pEntity->GetHealth() > 0 && !pEntity->IsDormant())
					{
						*(int*)((uintptr_t)pEntity + 0xA30) = g_pGlobals->framecount; //we'll skip occlusion checks now
						*(int*)((uintptr_t)pEntity + 0xA28) = 0;//clear occlusion flags
					}
				}
			}
		}
		

		clientVMT->GetOriginalMethod<FrameStageNotifyFn>(36)(curStage);

	
		//skinchanger
		if (curStage == FRAME_NET_UPDATE_POSTDATAUPDATE_START && MiscElements.Misc_Windows_Skinchanger->Checked)
		{
			UINT* hWeapons = G::LocalPlayer->GetWeapons();
			for (int i = 0; hWeapons[i]; i++)
			{
				CBaseCombatWeapon* weapon = (CBaseCombatWeapon*)g_pEntitylist->GetClientEntityFromHandle(hWeapons[i]);
				if (!weapon)
					continue;

				CBaseAttributableItem* attrib_item = (CBaseAttributableItem*)weapon;
				if (!attrib_item)
					continue;

				if (weapon->IsKnife())
				{
					if (GlobalSkinchanger->Skinchanger_Knife->SelectedIndex > 0)
					{
						switch (GlobalSkinchanger->Skinchanger_Knife->SelectedIndex)
						{
						case 1:
							*attrib_item->GetItemDefinitionIndex() = KNIFE_KARAMBIT;
							break;
						case 2:
							*attrib_item->GetItemDefinitionIndex() = KNIFE_BAYONET;
							break;
						case 3:
							*attrib_item->GetItemDefinitionIndex() = KNIFE_M9_BAYONET;
							break;
						case 4:
							*attrib_item->GetItemDefinitionIndex() = KNIFE_TACTICAL;
							break;
						case 5:
							*attrib_item->GetItemDefinitionIndex() = KNIFE_GUT;
							break;
						case 6:
							*attrib_item->GetItemDefinitionIndex() = KNIFE_FALCHION;
							break;
						case 7:
							*attrib_item->GetItemDefinitionIndex() = KNIFE_PUSH;
							break;
						case 8:
							*attrib_item->GetItemDefinitionIndex() = KNIFE_BUTTERFLY;
							break;
						case 9:
							*attrib_item->GetItemDefinitionIndex() = KNIFE_FLIP;
							break;
						case 10:
							*attrib_item->GetItemDefinitionIndex() = KNIFE_BOWIE;
							break;
						default:
							break;
						}

					}
					else
					{
						if (G::LocalPlayer->GetTeamNum() == 2)
							*attrib_item->GetItemDefinitionIndex() = KNIFE_T;
						else if (G::LocalPlayer->GetTeamNum() == 3)
							*attrib_item->GetItemDefinitionIndex() = KNIFE;
					}

					int i = *attrib_item->GetItemDefinitionIndex();

					*(int*)((DWORD)weapon + offys.m_nModeIndex) = get_model_index(i);


					CBaseEntity* view_model = g_pEntitylist->GetClientEntityFromHandle_D(*(PDWORD)((DWORD)G::LocalPlayer + offys.m_hViewModel));
					if (view_model)
					{

						DWORD hViewModelWeapon = *(PDWORD)((DWORD)view_model + offys.m_hWeapon);
						CBaseAttributableItem* pViewModelWeapon = (CBaseAttributableItem*)g_pEntitylist->GetClientEntityFromHandle_D(hViewModelWeapon);


						if (pViewModelWeapon)
						{

							//*(int*)((DWORD)pViewModelWeapon + offys.m_nModeIndex) = get_model_index(i);
							int p = *reinterpret_cast<int*>((DWORD)pViewModelWeapon + offys.m_nModeIndex);

						}
					}
					//attrib_item->SetModelIndex(get_model_index(*attrib_item->GetItemDefinitionIndex()));
				}
				

				g_Skinchanger->ApplyCustomSkin(attrib_item);


			}
			if (GlobalSkinchanger->Reload)
			{
				static auto CvarFulllUpdate = g_pCvar->FindVar("cl_fullupdate");
				if (!g_pEngine->IsInGame())
					return;

				//if (CvarFulllUpdate)
					//CvarFulllUpdate->m_fnChangeCallbacksV1();
				GlobalSkinchanger->Reload = false;
			}
		}


		if (curStage == FRAME_NET_UPDATE_POSTDATAUPDATE_START)
		{
			for (int i = 1; i < g_pGlobals->maxClients; i++)
			{
				if (i == g_pEngine->GetLocalPlayer())
					continue;
				CBaseEntity* pEntity = g_pEntitylist->GetClientEntity(i);
				if (pEntity && pEntity->GetHealth() > 0)
				{
					if (pEntity->IsDormant())
						continue;




					if (G::LocalPlayer)
						g_Resolver->Run(pEntity);

				}
				else
				{
					g_BacktrackHelper->PlayerRecord[i].records.clear();
				}
			}
		}

		

		if (curStage == FRAME_RENDER_START)
		{
			for (int i = 1; i < g_pGlobals->maxClients; i++)
			{
				CBaseEntity* pEntity = g_pEntitylist->GetClientEntity(i);
				if (pEntity)
				{
					if (pEntity->GetHealth() > 0 && !pEntity->IsDormant())
					{
						
						g_BacktrackHelper->UpdateBacktrackRecords(pEntity);
						g_BacktrackHelper->UpdateExtrapolationRecords(pEntity);

					}
				}
			}
		}
		
		

		if (curStage == FRAME_RENDER_START && G::LocalPlayer && G::LocalPlayer->GetHealth() > 0 && !b_IsThirdPerson)
		{

			if (VisualElements.Visual_Misc_NoRecoil->Checked)
			{
				*aim_punch = oldAimPunch;
				*view_punch = oldViewPunch;
			}
		}
	
}