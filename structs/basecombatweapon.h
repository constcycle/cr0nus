#pragma once
enum WEAPONCLASS
{
	WEPCLASS_INVALID,
	WEPCLASS_RIFLE,
	WEPCLASS_PISTOL,
	WEPCLASS_SHOTGUN,
	WEPCLASS_SNIPER,
	WEPCLASS_SMG,
	WEPCLASS_MACHINEGUN,
	WEPCLASS_KNIFE,
};
enum ItemDefinitionIndex : int {
	DEAGLE = 1,
	ELITE = 2,
	FIVESEVEN = 3,
	GLOCK = 4,
	AK47 = 7,
	AUG = 8,
	AWP = 9,
	FAMAS = 10,
	G3SG1 = 11,
	GALILAR = 13,
	M249 = 14,
	M4A1 = 16,
	MAC10 = 17,
	P90 = 19,
	UMP45 = 24,
	XM1014 = 25,
	BIZON = 26,
	MAG7 = 27,
	NEGEV = 28,
	SAWEDOFF = 29,
	TEC9 = 30,
	TASER = 31,
	HKP2000 = 32,
	MP7 = 33,
	MP9 = 34,
	NOVA = 35,
	P250 = 36,
	SCAR20 = 38,
	SG556 = 39,
	SSG08 = 40,
	KNIFE = 42,
	FLASHBANG = 43,
	HEGRENADE = 44,
	SMOKEGRENADE = 45,
	MOLOTOV = 46,
	DECOY = 47,
	INCGRENADE = 48,
	C4 = 49,
	KNIFE_T = 59,
	M4A1_SILENCER = 60,
	USP_SILENCER = 61,
	CZ75A = 63,
	REVOLVER = 64,
	KNIFE_BAYONET = 500,
	KNIFE_FLIP = 505,
	KNIFE_GUT = 506,
	KNIFE_KARAMBIT = 507,
	KNIFE_M9_BAYONET = 508,
	KNIFE_TACTICAL = 509,
	KNIFE_FALCHION = 512,
	KNIFE_BUTTERFLY = 515,
	KNIFE_PUSH = 516,
	KNIFE_BOWIE = 514
};
struct CHudTexture
{
	char	szShortName[64];	//0x0000
	char	szTextureFile[64];	//0x0040
	bool	bRenderUsingFont;	//0x0080
	bool	bPrecached;			//0x0081
	char	cCharacterInFont;	//0x0082
	BYTE	pad_0x0083;			//0x0083
	int		hFont;				//0x0084
	int		iTextureId;			//0x0088
	float	afTexCoords[4];		//0x008C
	int		iPosX[4];			//0x009C
}; //Size=0x00AC
struct CSWeaponInfo
{
	BYTE		pad_0x0000[4];			//0x0000
	bool		bParsedScript;			//0x0004
	bool		bLoadedHudElements;		//0x0005
	char		szClassName[80];		//0x0006
	char		szPrintName[80];		//0x0056
	char		szViewModel[80];		//0x00A6
	char		szWorldModel[80];		//0x00F6
	char		szBulletType[32];		//0x0146
	char		szWorldDroppedModel[80];//0x0166
	char		szAnimationPrefix[16];	//0x01B6
	BYTE		pad_0x01C6[2];			//0x01C6
	int			iSlot;					//0x01C8
	int			iPosition;				//0x01CC
	int			iMaxClip1;				//0x01D0
	int			iMaxClip2;				//0x01D4
	int			iDefaultClip1;			//0x01D8
	int			iDefaultClip2;			//0x01DC
	int			iWeight;				//0x01E0
	int			iRumbleEffect;			//0x01E4
	bool		bAutoSwitchTo;			//0x01E8
	bool		bAutoSwitchFrom;		//0x01E9
	BYTE		pad_0x01EA[2];			//0x01EA
	int			iFlags;					//0x01EC
	char		szAmmo1[16];			//0x01F0
	char		szAmmo2[16];			//0x0200
	char		szAIAddOn[80];			//0x0210
	char		aShootSounds[17][80];	//0x0260
	char PAD_NEW[80];
	int			iAmmoType;
	int			iAmmo2Type;
	bool		bMeleeWeapon;
	bool		bBuiltRightHanded;
	bool		bAllowFlipping;
	BYTE		pad_0x07BB;
	int			iSpriteCount;
	CHudTexture* iconWeaponS;
	CHudTexture* iconWeapon;
	CHudTexture* iconAmmo;
	CHudTexture* iconAmmo2;
	CHudTexture* iconCrosshair;
	CHudTexture* iconAutoaim;
	CHudTexture* iconZoomedCrosshair;
	CHudTexture* iconZoomedAutoaim;
	CHudTexture* iconWeaponSmall;
	bool		bShowUsageHint;
	BYTE		pad_0x07E5[3];
	CSWeaponType iWeaponType;
	int			iTeam;
	int			iWeaponID;
	bool		bFullAuto;
	BYTE		pad_0x07F5[3];
	float		flHeatPerShot;
	int			iWeaponPrice;
	float		flArmorRatio;
	float		flMaxPlayerSpeed;
	float		flMaxPlayerSpeedAlt;
	int			iCrosshairMinDistance;
	int			iCrosshairDeltaDistance;
	float		flPenetration;
	int			iDamage;
	float		flRange;
	float		flRangeModifier;
	int			iBullets;
	float		flCycleTime;
	float		flCycleTimeAlt;
	char		szHeatEffect[80];
	float		aflSmokeColor[3];
	char		szMuzzleFlashEffect_1stPerson[80];
	char		szMuzzleFlashEffect_3rdPerson[80];
	char		szEjectBrassEffect[80];
	char		szTracerEffect[80];
	int			iTracerFrequency;
	float		flSpread;
	float		flSpreadAlt;
	float		flInaccuracyCrouch;
	float		flInaccuracyCrouchAlt;
	float		flInaccuracyStand;
	float		flInaccuracyStandAlt;
	float		flInaccuracyJump;
	float		flInaccuracyJumpAlt;
	float		flInaccuracyLand;
	float		flInaccuracyLandAlt;
	float		flInaccuracyLadder;
	float		flInaccuracyLadderAlt;
	float		flInaccuracyFire;
	float		flInaccuracyFireAlt;
	float		flInaccuracyMove;
	float		flInaccuracyMoveAlt;
	float		flRecoveryTimeStand;
	float		flRecoveryTimeCrouch;
	float		flInaccuracyReload;
	float		flInaccuracyAltSwitch;
	float		flRecoilAngle;
	float		flRecoilAngleAlt;
	float		flRecoilAngleVariance;
	float		flRecoilAngleVarianceAlt;
	float		flRecoilMagnitude;
	float		flRecoilMagnitudeAlt;
	float		flRecoilMagnitudeVariance;
	float		flRecoilMagnitudeVarianceAlt;
	int			iRecoilSeed;
	float		flFlinchVelocityModifierLarge;
	float		flFlinchVelocityModifierSmall;
	float		flTimeToIdle;
	float		flIdleInterval;
	float		recoilTable[2][128];
	int			iZoomLevels;
	int			iZoomFOV[3];
	float		fZoomTime[3];
	bool		bHideViewModelZoomed;
	char		szZoomInSound[80];
	char		szZoomOutSound[80];
	BYTE		pad_0x0F11[3];
	float		flBotAudibleRange;
	bool		bCanUseWithShield;
	char		szWrongTeamMsg[32];
	char		szAnimPrefix[16];
	char		szShieldViewModel[64];
	char		szAddonModel[80];
	char		szAddonLocation[80];
	char		szSilencerModel[80];
	BYTE		pad_0x1079[3];
	float		flAddonScale;
	float		flThrowVelocity;
	int			iKillAward;
	BYTE		pad_0x1088[8];
};
class CBomb
{
public:
	float GetC4BlowTime()
	{
		return *reinterpret_cast<float*>((DWORD)this + offys.m_flC4Blow);
	}
	HANDLE GetOwnerEntity()
	{
		return *reinterpret_cast<HANDLE*>((DWORD)this + offys.m_hOwnerEntity);
	}
	bool IsDefused()
	{
		return *reinterpret_cast<bool*>((DWORD)this + offys.m_bBombDefused);
	}
};
class CGrenade
{
public:
	bool IsPinPulled()
	{
		return *reinterpret_cast<bool*>((DWORD)this + offys.m_bPinPulled);
	}
	float GetThrowTime()
	{
		return *reinterpret_cast<float*>((DWORD)this + offys.m_fThrowTime);
	}
};
class CBaseCombatWeapon
{
public:

	//CBaseViewModel* GetViewModel()
	//{
	//	if (!this)
	//		return nullptr;
		//static std::uintptr_t offset = GET_NETVAR(XorStr("DT_BasePlayer"), XorStr("m_hViewModel[0]"));
		//return (CBaseViewModel*)I::ClientCBaseEntityList->GetClientEntityFromHandle(GetFieldValue<CHandle<CBaseViewModel>>(offset));
	//}
	model_t* CBaseCombatWeapon::GetModel()
	{
		return *(model_t**)((DWORD)this + 0x6C);
	}
	Vector GetOrigin()
	{
		return *reinterpret_cast<Vector*>((DWORD)this + 0x0134);
	}
	HANDLE GetOwnerCBaseEntity()
	{
		return *reinterpret_cast<HANDLE*>((DWORD)this + 0x0148);
	}
	float NextPrimaryAttack()
	{
		return *reinterpret_cast<float*>((DWORD)this + 0x31D8);
	}
	float NextSecondaryAttack()
	{
		return *reinterpret_cast<float*>((DWORD)this + 0x31CC);
	}
	float GetAccuracyPenalty()
	{
		return *reinterpret_cast<float*>((DWORD)this + offys.m_fAccuracyPenalty);
	}
	int Clip1()
	{
		return *reinterpret_cast<int*>((DWORD)this + 0x3204);
	}
	int WeaponID()
	{
		return *reinterpret_cast<int*>((DWORD)this + 0x2F88);
		/*typedef int*(__thiscall* GetWeaponNameFn)(void*);
		GetWeaponNameFn Name = (GetWeaponNameFn)((*(PDWORD_PTR*)this)[458]);
		return Name(this);*/
	}
	float GetPostponeFireReadyTime()
	{
		return *reinterpret_cast<float*>((DWORD)this + offys.m_flPostponeFireReadyTime);
	}
	int GetZoomLevel()
	{
		return *reinterpret_cast<int*>((DWORD)this + 0x3330);
	}
	const char* GetWeaponName()
	{
		typedef const char*(__thiscall* GetWeaponNameFn)(void*);
		GetWeaponNameFn Name = (GetWeaponNameFn)((*(PDWORD_PTR*)this)[378]);
		return Name(this);
	}
	std::string GetName(bool Ammo)
	{
		const char* name = GetWeaponName();
		std::string Name = name;
		std::string NName;
			NName = Name.substr(7, Name.length() - 7);
	

		if (Ammo && !this->IsMiscWeapon())
		{
			char buffer[32]; sprintf_s(buffer, " [%i]", Clip1());
			NName.append(buffer);
			return NName;
		}
		return NName;
	}
	char* GetGunIcon()
	{
		int WeaponId = this->WeaponID();
		switch (WeaponId)
		{
		case KNIFE:
		case 500:
		case 505:
		case 506:
		case 507:
		case 508:
		case 509:
		case 512:
		case 514:
		case 515:
		case 516:
			return "]";
		case DEAGLE:
			return "A";
		case ELITE:
			return "B";
		case FIVESEVEN:
			return "C";
		case GLOCK:
			return "D";
		case HKP2000:
			return "E";
		case P250:
			return "F";
		case USP_SILENCER:
			return "G";
		case TEC9:
			return "H";
		case CZ75A:
			return "I";
		case REVOLVER:
			return "J";
		case MAC10:
			return "K";
		case UMP45:
			return "L";
		case BIZON:
			return "M";
		case MP7:
			return "N";
		case MP9:
			return "O";
		case P90:
			return "P";
		case GALILAR:
			return "Q";
		case FAMAS:
			return "R";
		case M4A1_SILENCER:
			return "S";
		case M4A1:
			return "T";
		case AUG:
			return "U";
		case SG556:
			return "V";
		case AK47:
			return "W";
		case G3SG1:
			return "X";
		case SCAR20:
			return "Y";
		case AWP:
			return "Z";
		case SSG08:
			return "a";
		case XM1014:
			return "b";
		case SAWEDOFF:
			return "c";
		case MAG7:
			return "d";
		case NOVA:
			return "e";
		case NEGEV:
			return "f";
		case M249:
			return "g";
		case TASER:
			return "h";
		case FLASHBANG:
			return "i";
		case HEGRENADE:
			return "j";
		case SMOKEGRENADE:
			return "k";
		case MOLOTOV:
			return "l";
		case DECOY:
			return "m";
		case INCGRENADE:
			return "n";
		case C4:
			return "o";
		default:
			return " ";
		}
	}
	int GetWeaponType()
	{
		if (!this) return WEPCLASS_INVALID;
		auto id = this->WeaponID();
		switch (id)
		{
		case DEAGLE:
		case ELITE:
		case FIVESEVEN:
		case HKP2000:
		case USP_SILENCER:
		case CZ75A:
		case TEC9:
		case REVOLVER:
		case GLOCK:
		case P250:
			return WEPCLASS_PISTOL;
			break;
		case AK47:
		case M4A1:
		case M4A1_SILENCER:
		case GALILAR:
		case AUG:
		case FAMAS:
		case SG556:
			return WEPCLASS_RIFLE;
			break;
		case P90:
		case BIZON:
		case MP7:
		case MP9:
		case MAC10:
		case UMP45:
			return WEPCLASS_SMG;
			break;
		case AWP:
		case G3SG1:
		case SCAR20:
		case SSG08:
			return WEPCLASS_SNIPER;
			break;
		case NEGEV:
		case M249:
			return WEPCLASS_MACHINEGUN;
			break;
		case MAG7:
		case SAWEDOFF:
		case NOVA:
		case XM1014:
			return WEPCLASS_SHOTGUN;
			break;
		case KNIFE:
		case KNIFE_BAYONET:
		case KNIFE_BUTTERFLY:
		case KNIFE_FALCHION:
		case KNIFE_FLIP:
		case KNIFE_GUT:
		case KNIFE_KARAMBIT:
		case KNIFE_TACTICAL:
		case KNIFE_M9_BAYONET:
		case KNIFE_PUSH:
		case KNIFE_BOWIE:
		case KNIFE_T:
			return WEPCLASS_KNIFE;
			break;

		default:
			return WEPCLASS_INVALID;
		}
	}
	float GetInaccuracy()
	{
		typedef float(__thiscall* GetInaccuracyFn)(void*);
		GetInaccuracyFn Inaccuracy = (GetInaccuracyFn)((*(PDWORD_PTR*)this)[483]); //459
		return Inaccuracy(this);
	}
	float GetFloatRecoilIndex()
	{
		return *reinterpret_cast<float*>((DWORD)this + 0x32D0);
	}

	float GetSpread()
	{
		typedef float(__thiscall* GetSpreadFn)(void*);
		GetSpreadFn Spread = (GetSpreadFn)((*(PDWORD_PTR*)this)[484]);//460
		return Spread(this);
	}

	void UpdateAccuracyPenalty()
	{
		typedef void(__thiscall* UpdateAccuracyPenaltyFn)(void*);
		UpdateAccuracyPenaltyFn Update = (UpdateAccuracyPenaltyFn)((*(PDWORD_PTR*)this)[485]);
		return Update(this);
	}
	bool IsRifle()
	{
		int iWeaponID = WeaponID();
	}
	bool IsScopeable()
	{
		int iWeaponID = WeaponID();
		return (iWeaponID == 38 || iWeaponID == 11 || iWeaponID == 9 || iWeaponID == 40 || iWeaponID == 8 || iWeaponID == SG556);
	}
	bool IsSniper()
	{
		int iWeaponID = WeaponID();
		return (iWeaponID == SSG08 || iWeaponID == AWP || iWeaponID == SCAR20 || iWeaponID == G3SG1);
	}
	bool IsPistol()
	{
		int iWeaponID = WeaponID();
		return (iWeaponID == GLOCK || iWeaponID == HKP2000
			|| iWeaponID == P250 || iWeaponID == DEAGLE
			|| iWeaponID == ELITE || iWeaponID == TEC9 || iWeaponID == USP_SILENCER
			|| iWeaponID == FIVESEVEN);
	}
	bool IsMiscWeapon()
	{
		int iWeaponID = WeaponID();
		return (iWeaponID == KNIFE
			|| iWeaponID == C4
			|| iWeaponID == HEGRENADE || iWeaponID == DECOY
			|| iWeaponID == FLASHBANG || iWeaponID == MOLOTOV
			|| iWeaponID == SMOKEGRENADE || iWeaponID == INCGRENADE || iWeaponID == KNIFE_T
			|| iWeaponID == 500 || iWeaponID == 505 || iWeaponID == 506
			|| iWeaponID == 507 || iWeaponID == 508 || iWeaponID == 509
			|| iWeaponID == 515);
	}
	bool IsGrenade()
	{

		int iWeaponID = WeaponID();
		return (iWeaponID == HEGRENADE || iWeaponID == DECOY
			|| iWeaponID == FLASHBANG || iWeaponID == MOLOTOV
			|| iWeaponID == SMOKEGRENADE || iWeaponID == INCGRENADE);
	}
	bool IsKnife()
	{
		int iWeaponID = WeaponID();
		return (iWeaponID == 42 || iWeaponID == 59 || iWeaponID == 41
			|| iWeaponID == 500 || iWeaponID == 505 || iWeaponID == 506
			|| iWeaponID == 507 || iWeaponID == 508 || iWeaponID == 509
			|| iWeaponID == 515);
	}
	float GetPenetration()
	{
		return *reinterpret_cast<float*>((DWORD)this + 0x7C4);
	}
	float GetDamage()
	{
		return *reinterpret_cast<float*>((DWORD)this + 0x7C8);
	}
	float GetRange()
	{
		return *reinterpret_cast<float*>((DWORD)this + 0x7CC);
	}
	float GetRangeModifier()
	{
		return *reinterpret_cast<float*>((DWORD)this + 0x7D0);
	}
	float GetArmorRatio()
	{
		return *reinterpret_cast<float*>((DWORD)this + 0x7AC);
	}
	CSWeaponInfo* GetCSWpnData();
};
class CBaseAttributableItem {
public:
	IClientNetworkable* GetNetworkable()
	{
		return reinterpret_cast<IClientNetworkable*>((DWORD)this + 0x8);
	}
	void SetModelIndex(int index)
	{
		typedef void(__thiscall* Fn)(void*, int);
		return getvfunc<Fn>(this, 75)(this, index);
	}

	inline int* GetItemDefinitionIndex() {
		// DT_BaseAttributableItem -> m_AttributeManager: 0x2D50
		// DT_BaseAttributableItem -> m_AttributeManager -> m_Item: 0x40
		// DT_BaseAttributableItem -> m_AttributeManager -> m_Item -> m_iItemDefinitionIndex: 0x1D0
		return (int*)((DWORD)this +offys.m_iItemDefinitionIndex);
	}

	inline int* GetItemIDHigh() {
		// DT_BaseAttributableItem -> m_AttributeManager: 0x2D50
		// DT_BaseAttributableItem -> m_AttributeManager -> m_Item: 0x40
		// DT_BaseAttributableItem -> m_AttributeManager -> m_Item -> m_iItemIDHigh: 0x1E8
		return (int*)((DWORD)this + offys.m_iItemIDHigh);
	}
	inline int* GetItemIDLow()
	{
		return (int*)((DWORD)this + offys.m_iItemIDLow);
	}

	inline int* GetEntityQuality() {
		// DT_BaseAttributableItem -> m_AttributeManager: 0x2D50
		// DT_BaseAttributableItem -> m_AttributeManager -> m_Item: 0x40
		// DT_BaseAttributableItem -> m_AttributeManager -> m_Item -> m_iCBaseEntityQuality: 0x1D4
		return (int*)((DWORD)this + offys.m_iEntityQuality);
	}

	inline char* GetCustomName() {
		// DT_BaseAttributableItem -> m_AttributeManager: 0x2D50
		// DT_BaseAttributableItem -> m_AttributeManager -> m_Item: 0x40
		// DT_BaseAttributableItem -> m_AttributeManager -> m_Item -> m_szCustomName: 0x264
		return (char*)((DWORD)this + offys.m_szCustomName);
	}

	inline int* GetOriginalOwnerXuidLow() {
		// DT_BaseAttributableItem -> m_OriginalOwnerXuidLow: 0x3138
		return (int*)((DWORD)this + offys.m_OriginalOwnerXuidLow);
	}

	inline int* GetOriginalOwnerXuidHigh() {
		// DT_BaseAttributableItem -> m_OriginalOwnerXuidHigh: 0x313C
		return (int*)((DWORD)this +offys.m_OriginalOwnerXuidHigh);
	}

	inline int* GetFallbackPaintKit() {
		// DT_BaseAttributableItem -> m_nFallbackPaintKit: 0x3140
		return (int*)((DWORD)this + offys.m_nFallbackPaintKit);
	}

	inline int* GetFallbackSeed() {
		// DT_BaseAttributableItem -> m_nFallbackSeed: 0x3144
		return (int*)((DWORD)this + offys.m_nFallbackSeed);
	}

	inline float* GetFallbackWear() {
		// DT_BaseAttributableItem -> m_flFallbackWear: 0x3148
		return (float*)((DWORD)this + offys.m_flFallbackWear);
	}

	inline int* GetFallbackStatTrak() {
		// DT_BaseAttributableItem -> m_nFallbackStatTrak: 0x314C
		return (int*)((DWORD)this + offys.m_nFallbackStatTrak);
	}
	inline int* GetAccountID()
	{
		return (int*)((DWORD)this + offys.m_nAccountID);
	}
	
};

class CBaseViewModel
{
public:
	CBaseViewModel(void);
	~CBaseViewModel(void);


	bool IsViewable(void) { return false; }

	virtual void					UpdateOnRemove(void);

	// Weapon client handling
	virtual void			SendViewModelMatchingSequence(int sequence);
	virtual void			SetWeaponModel(const char *pszModelname, CBaseCombatWeapon *weapon);

	void SendViewModelMatchingSequenceManual(int sequence)
	{
		typedef void(__thiscall* OriginalFn)(void*, int);
		return getvfunc<OriginalFn>(this, 241)(this, sequence);
	}

};
enum DataUpdateType_t
{
	DATA_UPDATE_CREATED = 0,	// indicates it was created +and+ entered the pvs
								//	DATA_UPDATE_ENTERED_PVS,
								DATA_UPDATE_DATATABLE_CHANGED,
								//	DATA_UPDATE_LEFT_PVS,
								//	DATA_UPDATE_DESTROYED,		// FIXME: Could enable this, but it's a little worrying
								// since it changes a bunch of existing code
};
class C_BasePlayer
{
public:


	C_BasePlayer();
	virtual			~C_BasePlayer();
	virtual void	Spawn(void);
	virtual void	SharedSpawn(); // Shared between client and server.
	void*		Classify(void) { return 0; }
	// IClientCBaseEntity overrides.
	virtual void	OnPreDataChanged(DataUpdateType_t updateType);
	virtual void	OnDataChanged(DataUpdateType_t updateType);
	virtual void	PreDataUpdate(DataUpdateType_t updateType);
	virtual void	PostDataUpdate(DataUpdateType_t updateType);
	virtual void	ReceiveMessage(int classID, void* &msg);
	virtual void	OnRestore();
	virtual void	MakeTracer(const Vector &vecTracerSrc, const void* &tr, int iTracerType);
	virtual void	GetToolRecordingState(KeyValues *msg);
	void	SetAnimationExtension(const char *pExtension);
	CBaseViewModel			*GetViewModel(int viewmodelindex = 0);
};