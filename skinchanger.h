#pragma once
struct EconomyItemCfg {
	int iItemDefinitionIndex = 0;
	int nFallbackPaintKit = 0;
	int nFallbackSeed = 0;
	int nFallbackStatTrak = -1;
	int iEntityQuality = 4;
	char* szCustomName = nullptr;
	float flFallbackWear = 0.1f;
};

extern std::unordered_map<std::string, EconomyItemCfg> g_SkinChangerCfg;
class CSkinchanger
{
public:
	bool ApplyCustomSkin(CBaseAttributableItem* attrib_item);

};
extern void ViewModel_Index_Proxy(CRecvProxyData *pDataConst, void *pStruct, void *pOut);
extern CSkinchanger* g_Skinchanger;