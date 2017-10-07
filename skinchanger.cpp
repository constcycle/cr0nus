#include "sdk.h"
#include <unordered_map>
#include "skinchanger.h"
#include <random>
#include "Menu.h"
#include "global.h"
CSkinchanger* g_Skinchanger = new CSkinchanger;

std::unordered_map<std::string, EconomyItemCfg> g_SkinChangerCfg;

char easytolower(char in) {
	if (in <= 'Z' && in >= 'A')
		return in - ('Z' - 'z');
	return in;
}
int _get_model_index(int item_def_index)
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
void ViewModel_Index_Proxy(CRecvProxyData *pDataConst, void *pStruct, void *pOut)
{
	if (((*(int*)((DWORD)pDataConst + 0x8) == g_pModelInfo->GetModelIndex("models/weapons/v_knife_default_t.mdl")) || *(int*)((DWORD)pDataConst + 0x8) == g_pModelInfo->GetModelIndex("models/weapons/v_knife_default_ct.mdl")))
	{
		auto KnifeType = GlobalSkinchanger->Skinchanger_Knife->SelectedIndex;
		if (KnifeType > 0)
		{
			UINT* hWeapons = G::LocalPlayer->GetWeapons();
			for (int i = 0; hWeapons[i]; i++)
			{
				CBaseCombatWeapon* weapon = (CBaseCombatWeapon*)g_pEntitylist->GetClientEntityFromHandle(hWeapons[i]/* & 0xFFF*/);
				if (!weapon)
					continue;

				CBaseAttributableItem* attrib_item = (CBaseAttributableItem*)weapon;
				if (!attrib_item)
					continue;

				if (weapon->IsKnife())
				{
					*(int*)((DWORD)pDataConst + 0x8) = _get_model_index(weapon->WeaponID());
				}
			}
		}
	}
	
	*(int*)((DWORD)pOut) = *(int*)((DWORD)pDataConst + 0x8);
}

bool CSkinchanger::ApplyCustomSkin(CBaseAttributableItem* attrib_item) {
	// Get the weapon entity from the provided handle.



	if (!attrib_item)
		return false;

	// Get the weapons item definition index.
	int nWeaponIndex = *attrib_item->GetItemDefinitionIndex();


	std::string weaponstr;
	switch (nWeaponIndex)
	{
	case KNIFE_BAYONET:
		weaponstr.assign("WEAPON_BAYONET");
		break;
	case KNIFE_M9_BAYONET:
		weaponstr.assign("WEAPON_KNIFE_M9_BAYONET");
		break;
	case KNIFE_FLIP:
		weaponstr.assign("WEAPON_KNIFE_FLIP");
		break;
	case KNIFE_FALCHION:
		weaponstr.assign("WEAPON_KNIFE_FALCHION");
		break;
	case KNIFE_KARAMBIT:
		weaponstr.assign("WEAPON_KNIFE_KARAMBIT");
		break;
	case KNIFE_GUT:
		weaponstr.assign("WEAPON_KNIFE_GUT");
		break;
	case KNIFE_TACTICAL:
		weaponstr.assign("WEAPON_KNIFE_TACTICAL");
		break;
	case KNIFE_BOWIE:
		weaponstr.assign("WEAPON_KNIFE_SURVIVAL_BOWIE");
		break;
	case KNIFE_BUTTERFLY:
		weaponstr.assign("WEAPON_KNIFE_BUTTERFLY");
		break;
	case REVOLVER:
		weaponstr.assign("WEAPON_REVOLVER");
		break;
	case KNIFE_PUSH:
		weaponstr.assign("WEAPON_KNIFE_PUSH");
		break;
	case DEAGLE:
		weaponstr.assign("WEAPON_DEAGLE");
		break;
	case ELITE:
		weaponstr.assign("WEAPON_ELITE");
		break;
	case FIVESEVEN:
		weaponstr.assign("WEAPON_FIVESEVEN");
		break;
	case GLOCK:
		weaponstr.assign("WEAPON_GLOCK");
		break;
	case AK47:
		weaponstr.assign("WEAPON_AK47");
		break;
	case AUG:
		weaponstr.assign("WEAPON_AUG");
		break;
	case AWP:
		weaponstr.assign("WEAPON_AWP");
		break;
	case FAMAS:
		weaponstr.assign("WEAPON_FAMAS");
		break;
	case G3SG1:
		weaponstr.assign("WEAPON_G3SG1");
		break;
	case GALILAR:
		weaponstr.assign("WEAPON_GALILAR");
		break;
	case M249:
		weaponstr.assign("WEAPON_M249");
		break;
	case M4A1_SILENCER:
		weaponstr.assign("WEAPON_M4A1_SILENCER");
		break;
	case M4A1:
		weaponstr.assign("WEAPON_M4A1");
		break;
	case MAC10:
		weaponstr.assign("WEAPON_MAC10");
		break;
	case P90:
		weaponstr.assign("WEAPON_P90");
		break;
	case UMP45:
		weaponstr.assign("WEAPON_UMP");
		break;
	case XM1014:
		weaponstr.assign("WEAPON_XM1014");
		break;
	case BIZON:
		weaponstr.assign("WEAPON_BIZON");
		break;
	case MAG7:
		weaponstr.assign("WEAPON_MAG7");
		break;
	case NEGEV:
		weaponstr.assign("WEAPON_NEGEV");
		break;
	case SAWEDOFF:
		weaponstr.assign("WEAPON_SAWEDOFF");
		break;
	case TEC9:
		weaponstr.assign("WEAPON_TEC9");
		break;
	case HKP2000:
		weaponstr.assign("WEAPON_HKP2000");
		break;
	case MP7:
		weaponstr.assign("WEAPON_MP7");
		break;
	case MP9:
		weaponstr.assign("WEAPON_MP9");
		break;
	case NOVA:
		weaponstr.assign("WEAPON_NOVA");
		break;
	case P250:
		weaponstr.assign("WEAPON_P250");
		break;
	case SCAR20:
		weaponstr.assign("WEAPON_SCAR20");
		break;
	case SG556:
		weaponstr.assign("WEAPON_SG556");
		break;
	case SSG08:
		weaponstr.assign("WEAPON_SSG08");
		break;
	case USP_SILENCER:
		weaponstr.assign("WEAPON_USP_SILENCER");
		break;
	case KNIFE_T:
		//weaponstr.assign("WEAPON_hkp2000");
		break;
	case KNIFE:
		//weaponstr.assign("WEAPON_hkp2000");
		break;
	}

	std::transform(weaponstr.begin(), weaponstr.end(), weaponstr.begin(), easytolower);

	g_SkinChangerCfg[weaponstr].nFallbackPaintKit = SkinsUtil::data.find(weaponstr)->second;

	bool bStatTrack = GlobalSkinchanger->Skinchanger_Stattrack->Checked;

	int StatTrackCount = GlobalSkinchanger->Skinchanger_Stattrack_Kills->value;

	

	*attrib_item->GetFallbackPaintKit() = g_SkinChangerCfg[weaponstr].nFallbackPaintKit;//SkinsUtil::data.find(weaponstr)->second;//

	*attrib_item->GetFallbackSeed() = (int)GlobalSkinchanger->Skinchanger_Seed->value;//g_SkinChangerCfg[weaponstr].nFallbackSeed;

	if (((CBaseCombatWeapon*)attrib_item)->IsKnife())
		if (GlobalSkinchanger->Skinchanger_Knife->SelectedIndex > 0)
			*attrib_item->GetEntityQuality() = 3;
		else
			*attrib_item->GetEntityQuality() = 1;
	else
		*attrib_item->GetEntityQuality() = bStatTrack ? 4 : 0;

	if (bStatTrack)
		*attrib_item->GetFallbackStatTrak() = (int)GlobalSkinchanger->Skinchanger_Stattrack_Kills->value;//g_SkinChangerCfg[weaponstr].nFallbackStatTrak;
	else
		*attrib_item->GetFallbackStatTrak() = -1;
	*attrib_item->GetFallbackWear() = GlobalSkinchanger->Skinchanger_Wear->value;//g_SkinChangerCfg[weaponstr].flFallbackWear;


	
	//*attrib_item->GetAccountID() = pinfo.m_nXuidLow;
	*attrib_item->GetItemIDHigh() = -1; //-1
	//*attrib_item->GetItemIDLow() = 0;


	return true;
}