#include "hooks.h"
#include "Menu.h"
#include "global.h"
#include "MaterialHelper.h"
#include "xor.h"
#include "Render.h"
void __fastcall Hooks::DrawModelExecute(void* ecx, void* edx, void* * ctx, void *state, const ModelRenderInfo_t &pInfo, matrix3x4_t *pCustomBoneToWorld)
{
		if (!G::LocalPlayer)
		{
			modelrenderVMT->GetOriginalMethod<DrawModelExecuteFn>(21)(ecx, ctx, state, pInfo, pCustomBoneToWorld);
			return;
		}

		Color color;
		float flColor[3] = { 0.f };

		bool DontDraw = false;




		const char* ModelName = g_pModelInfo->GetModelName((model_t*)pInfo.pModel);

		CBaseEntity* pModelCBaseEntity = g_pEntitylist->GetClientEntity(pInfo.entity_index);
		CBaseEntity* pLocal = G::LocalPlayer;
		// Player Chams
		int ChamsStyle = VisualElements.Visual_Chams_Style->SelectedIndex;
		int HandsStyle = VisualElements.Visual_Misc_Hands->SelectedIndex;
		/*if (strstr(ModelName, XorStr("smoke")) && VisualElements.Visual_Misc_NoSmoke->Checked)
		{
			IMaterial* vistasmokev1 = g_pMaterialSystem->FindMaterial(XorStr("particle\\vistasmokev1\\vistasmokev1"), TEXTURE_GROUP_PARTICLE);
			IMaterial* vistasmokev1_emods = g_pMaterialSystem->FindMaterial(XorStr("particle\\vistasmokev1\\vistasmokev1_emods"), TEXTURE_GROUP_PARTICLE);
			IMaterial* vistasmokev1_emods_impactdust = g_pMaterialSystem->FindMaterial(XorStr("particle\\vistasmokev1\\vistasmokev1_emods_impactdust"), TEXTURE_GROUP_PARTICLE);
			IMaterial* vistasmokev1_fire = g_pMaterialSystem->FindMaterial(XorStr("particle\\vistasmokev1\\vistasmokev1_fire"), TEXTURE_GROUP_PARTICLE);
			IMaterial* vistasmokev1_fire2 = g_pMaterialSystem->FindMaterial(XorStr("particle\\vistasmokev1\\vistasmokev1_fire2"), TEXTURE_GROUP_PARTICLE);
			IMaterial* vistasmokev1_fire2_fogged = g_pMaterialSystem->FindMaterial(XorStr("particle\\vistasmokev1\\vistasmokev1_fire2_fogged"), TEXTURE_GROUP_PARTICLE);
			IMaterial* vistasmokev1_min_depth_nearcull = g_pMaterialSystem->FindMaterial(XorStr("particle\\vistasmokev1\\vistasmokev1_min_depth_nearcull"), TEXTURE_GROUP_PARTICLE);
			IMaterial* vistasmokev1_nearcull = g_pMaterialSystem->FindMaterial(XorStr("particle\\vistasmokev1\\vistasmokev1_nearcull"), TEXTURE_GROUP_PARTICLE);
			IMaterial* vistasmokev1_nearcull_fog = g_pMaterialSystem->FindMaterial(XorStr("particle\\vistasmokev1\\vistasmokev1_nearcull_fog"), TEXTURE_GROUP_PARTICLE);
			IMaterial* vistasmokev1_nearcull_nodepth = g_pMaterialSystem->FindMaterial(XorStr("particle\\vistasmokev1\\vistasmokev1_nearcull_nodepth"), TEXTURE_GROUP_PARTICLE);
			IMaterial* vistasmokev1_no_rgb = g_pMaterialSystem->FindMaterial(XorStr("particle\\vistasmokev1\\vistasmokev1_no_rgb"), TEXTURE_GROUP_PARTICLE);
			IMaterial* vistasmokev1_smokegrenade = g_pMaterialSystem->FindMaterial(XorStr("particle\\vistasmokev1\\vistasmokev1_smokegrenade"), TEXTURE_GROUP_PARTICLE);
			IMaterial* vistasmokev4_addself_nearcull = g_pMaterialSystem->FindMaterial(XorStr("particle\\vistasmokev1\\vistasmokev4_addself_nearcull"), TEXTURE_GROUP_PARTICLE);
			IMaterial* vistasmokev4_emods_nocul = g_pMaterialSystem->FindMaterial(XorStr("particle\\vistasmokev1\\vistasmokev4_emods_nocul"), TEXTURE_GROUP_PARTICLE);
			IMaterial* vistasmokev4_nearcull = g_pMaterialSystem->FindMaterial(XorStr("particle\\vistasmokev1\\vistasmokev4_nearcull"), TEXTURE_GROUP_PARTICLE);
			IMaterial* vistasmokev4_nocull = g_pMaterialSystem->FindMaterial(XorStr("particle\\vistasmokev1\\vistasmokev4_nocull"), TEXTURE_GROUP_PARTICLE);

			vistasmokev1->SetMaterialVarFlag(MATERIAL_VAR_NO_DRAW, true);
			vistasmokev1_emods->SetMaterialVarFlag(MATERIAL_VAR_NO_DRAW, true);
			vistasmokev1_emods_impactdust->SetMaterialVarFlag(MATERIAL_VAR_NO_DRAW, true);
			vistasmokev1_fire->SetMaterialVarFlag(MATERIAL_VAR_NO_DRAW, true);
			vistasmokev1_fire2->SetMaterialVarFlag(MATERIAL_VAR_NO_DRAW, true);
			vistasmokev1_fire2_fogged->SetMaterialVarFlag(MATERIAL_VAR_NO_DRAW, true);
			vistasmokev1_min_depth_nearcull->SetMaterialVarFlag(MATERIAL_VAR_NO_DRAW, true);
			vistasmokev1_nearcull->SetMaterialVarFlag(MATERIAL_VAR_NO_DRAW, true);
			vistasmokev1_nearcull_fog->SetMaterialVarFlag(MATERIAL_VAR_NO_DRAW, true);
			vistasmokev1_nearcull_nodepth->SetMaterialVarFlag(MATERIAL_VAR_NO_DRAW, true);
			vistasmokev1_no_rgb->SetMaterialVarFlag(MATERIAL_VAR_NO_DRAW, true);
			vistasmokev1_smokegrenade->SetMaterialVarFlag(MATERIAL_VAR_NO_DRAW, true);
			vistasmokev4_addself_nearcull->SetMaterialVarFlag(MATERIAL_VAR_NO_DRAW, true);
			vistasmokev4_emods_nocul->SetMaterialVarFlag(MATERIAL_VAR_NO_DRAW, true);
			vistasmokev4_nearcull->SetMaterialVarFlag(MATERIAL_VAR_NO_DRAW, true);
			vistasmokev4_nocull->SetMaterialVarFlag(MATERIAL_VAR_NO_DRAW, true);

			g_pModelRender->ForcedMaterialOverride(vistasmokev1);
			g_pModelRender->ForcedMaterialOverride(vistasmokev1_emods);
			g_pModelRender->ForcedMaterialOverride(vistasmokev1_emods_impactdust);
			g_pModelRender->ForcedMaterialOverride(vistasmokev1_fire);
			g_pModelRender->ForcedMaterialOverride(vistasmokev1_fire2);
			g_pModelRender->ForcedMaterialOverride(vistasmokev1_fire2_fogged);
			g_pModelRender->ForcedMaterialOverride(vistasmokev1_min_depth_nearcull);
			g_pModelRender->ForcedMaterialOverride(vistasmokev1_nearcull);
			g_pModelRender->ForcedMaterialOverride(vistasmokev1_nearcull_fog);
			g_pModelRender->ForcedMaterialOverride(vistasmokev1_nearcull_nodepth);
			g_pModelRender->ForcedMaterialOverride(vistasmokev1_no_rgb);
			g_pModelRender->ForcedMaterialOverride(vistasmokev1_smokegrenade);
			g_pModelRender->ForcedMaterialOverride(vistasmokev4_addself_nearcull);
			g_pModelRender->ForcedMaterialOverride(vistasmokev4_emods_nocul);
			g_pModelRender->ForcedMaterialOverride(vistasmokev4_nearcull);
			g_pModelRender->ForcedMaterialOverride(vistasmokev4_nocull);
		}*/
		if (strstr(ModelName, XorStr("flash")) && VisualElements.Visual_Misc_NoFlash->Checked)
		{
			IMaterial* Flash = g_pMaterialSystem->FindMaterial(XorStr("effects\\flashbang"), TEXTURE_GROUP_CLIENT_EFFECTS);
			IMaterial* FlashWhite = g_pMaterialSystem->FindMaterial(XorStr("effects\\flashbang_white"), TEXTURE_GROUP_CLIENT_EFFECTS);
			Flash->SetMaterialVarFlag(MATERIAL_VAR_NO_DRAW, true);
			FlashWhite->SetMaterialVarFlag(MATERIAL_VAR_NO_DRAW, true);
			g_pModelRender->ForcedMaterialOverride(Flash);
			g_pModelRender->ForcedMaterialOverride(FlashWhite);
		}
		if (((strstr(ModelName, XorStr("weapon"))) && !strstr(ModelName, XorStr("arms"))))
		{
			
				int Type = VisualElements.Visual_Misc_Weapon->SelectedIndex;
				if (Type != 0)
				{
					if (strstr(ModelName, XorStr("scope"))&&G::LocalPlayer->GetHealth() > 0 && G::LocalPlayer->IsScoped())
					{
						DontDraw = true;
					}
					else
					{
						if (Type == 1 || Type == 3)
						{
							float alpha = 1.f;

							float flColor[3];
							//PFIXES FOR WORKING COLORS

							flColor[0] = getR(ColorElements.Color_Misc_Weapon->color) / 255.f;
							flColor[1] = getG(ColorElements.Color_Misc_Weapon->color) / 255.f;
							flColor[2] = getB(ColorElements.Color_Misc_Weapon->color) / 255.f;
							
							g_pRenderView->SetColorModulation(flColor);
							g_pRenderView->SetBlend(alpha);
							if (Type == 1)
							{
								g_pModelRender->ForcedMaterialOverride(g_MaterialHelper->OpenLit);
							}
							else
								g_pModelRender->ForcedMaterialOverride(g_MaterialHelper->Wire);
							modelrenderVMT->GetOriginalMethod<DrawModelExecuteFn>(21)(ecx, ctx, state, pInfo, pCustomBoneToWorld);
						}
						else if (Type == 2)
							DontDraw = true;
					}

			}
		}
		if (HandsStyle != 0 && strstr(ModelName, XorStr("arms")))
		{
			int Type = HandsStyle;
			if (Type != 0)
			{
				if (Type == 1 || Type == 3)
				{
					float alpha = 1.f;

					float flColor[3];
					//PFIXES FOR WORKING COLORS
					flColor[0] = getR(ColorElements.Color_Misc_Hands->color) / 255.f;
					flColor[1] = getG(ColorElements.Color_Misc_Hands->color) / 255.f;
					flColor[2] = getB(ColorElements.Color_Misc_Hands->color) / 255.f;


					g_pRenderView->SetColorModulation(flColor);
					g_pRenderView->SetBlend(alpha);
					if (Type == 1)
					{
						g_pModelRender->ForcedMaterialOverride(g_MaterialHelper->OpenLit);
					}
					else
						g_pModelRender->ForcedMaterialOverride(g_MaterialHelper->Wire);


					modelrenderVMT->GetOriginalMethod<DrawModelExecuteFn>(21)(ecx, ctx, state, pInfo, pCustomBoneToWorld);
				}
				else if (Type == 2)
					DontDraw = true;

			}
		}
		if (VisualElements.Visual_Chams_Active->Checked)
		{
			if (strstr(ModelName, XorStr("models/player")))
			{
				if (pModelCBaseEntity)
				{
					if (G::LocalPlayer && (!VisualElements.Visual_Chams_EnemyOnly->Checked ||
						pModelCBaseEntity->GetTeamNum() !=G::LocalPlayer->GetTeamNum()))
					{
						IMaterial *covered = ChamsStyle == 1 || ChamsStyle == 2? g_MaterialHelper->CoveredLit : g_MaterialHelper->CoveredFlat;
						IMaterial *open = ChamsStyle == 1 || ChamsStyle == 2 ? g_MaterialHelper->OpenLit : g_MaterialHelper->OpenFlat;


						CBaseEntity *local = pLocal;
						if (local)
						{
							if (pModelCBaseEntity->GetHealth() > 0 /*&& pModelCBaseEntity->GetTeamNum() != local->GetTeamNum()*/)
							{
								float alpha = 1.f;

								if (ChamsStyle == 2)
									alpha = 0.20f;

								if (pModelCBaseEntity->GunGameImmunity())
									alpha /= 2.f;



								if (pModelCBaseEntity->GetTeamNum() == 3)
								{
									flColor[0] = getR(ColorElements.Color_CT_Invisible_Cham->color) / 255.f;
									flColor[1] = getG(ColorElements.Color_CT_Invisible_Cham->color) / 255.f;
									flColor[2] = getB(ColorElements.Color_CT_Invisible_Cham->color) / 255.f;
								}
								else
								{

									flColor[0] = getR(ColorElements.Color_T_Invisible_Cham->color) / 255.f;
									flColor[1] = getG(ColorElements.Color_T_Invisible_Cham->color) / 255.f;
									flColor[2] = getB(ColorElements.Color_T_Invisible_Cham->color) / 255.f;
								}
									g_pRenderView->SetColorModulation(flColor);
									g_pRenderView->SetBlend(alpha);
									g_pModelRender->ForcedMaterialOverride(covered);

								modelrenderVMT->GetOriginalMethod<DrawModelExecuteFn>(21)(ecx, ctx, state, pInfo, pCustomBoneToWorld);

								if (pModelCBaseEntity->GetTeamNum() == 3)
								{
									flColor[0] = getR(ColorElements.Color_CT_Visible_Cham->color) / 255.f;
									flColor[1] = getG(ColorElements.Color_CT_Visible_Cham->color) / 255.f;
									flColor[2] = getB(ColorElements.Color_CT_Visible_Cham->color) / 255.f;
								}
								else
								{

									flColor[0] = getR(ColorElements.Color_T_Visible_Cham->color) / 255.f;
									flColor[1] = getG(ColorElements.Color_T_Visible_Cham->color) / 255.f;
									flColor[2] = getB(ColorElements.Color_T_Visible_Cham->color) / 255.f;
								}

								g_pRenderView->SetColorModulation(flColor);
								g_pRenderView->SetBlend(alpha);
								g_pModelRender->ForcedMaterialOverride(open);
							}
							else
							{
								color.SetColor(255, 255, 255, 255);
								g_MaterialHelper->ForceMaterial(color, open);
							}
						}
					}
				}
			}



		}

		if (!DontDraw)
			modelrenderVMT->GetOriginalMethod<DrawModelExecuteFn>(21)(ecx, ctx, state, pInfo, pCustomBoneToWorld);
		g_pModelRender->ForcedMaterialOverride(NULL);
}