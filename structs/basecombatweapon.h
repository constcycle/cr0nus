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
class CSWeaponInfo
{
public:

    virtual ~CSWeaponInfo()
    {
    };

    char*		consoleName;			// 0x0004
    char		pad_0008[ 12 ];			// 0x0008
    int			iMaxClip1;				// 0x0014
    int			iMaxClip2;				// 0x0018
    int			iDefaultClip1;			// 0x001C
    int			iDefaultClip2;			// 0x0020
    char		pad_0024[ 8 ];			// 0x0024
    char*		szWorldModel;			// 0x002C
    char*		szViewModel;			// 0x0030
    char*		szDroppedModel;			// 0x0034
    char		pad_0038[ 4 ];			// 0x0038
    char*		N0000023E;				// 0x003C
    char		pad_0040[ 56 ];			// 0x0040
    char*		szEmptySound;			// 0x0078
    char		pad_007C[ 4 ];			// 0x007C
    char*		szBulletType;			// 0x0080
    char		pad_0084[ 4 ];			// 0x0084
    char*		szHudName;				// 0x0088
    char*		szWeaponName;			// 0x008C
    char		pad_0090[ 56 ];			// 0x0090
    int 		WeaponType;				// 0x00C8
    int			iWeaponPrice;			// 0x00CC
    int			iKillAward;				// 0x00D0
    char*		szAnimationPrefix;		// 0x00D4
    float		flCycleTime;			// 0x00D8
    float		flCycleTimeAlt;			// 0x00DC
    float		flTimeToIdle;			// 0x00E0
    float		flIdleInterval;			// 0x00E4
    bool		bFullAuto;				// 0x00E8
    char		pad_0x00E5[ 3 ];			// 0x00E9
    int			iDamage;				// 0x00EC
    float		flArmorRatio;			// 0x00F0
    int			iBullets;				// 0x00F4
    float		flPenetration;			// 0x00F8
    float		flFlinchVelocityModifierLarge;	// 0x00FC
    float		flFlinchVelocityModifierSmall;	// 0x0100
    float		flRange;				// 0x0104
    float		flRangeModifier;		// 0x0108
    float		flThrowVelocity;		// 0x010C
    char		pad_0x010C[ 12 ];			// 0x0110
    bool		bHasSilencer;			// 0x011C
    char		pad_0x0119[ 3 ];			// 0x011D
    char*		pSilencerModel;			// 0x0120
    int			iCrosshairMinDistance;	// 0x0124
    int			iCrosshairDeltaDistance;// 0x0128 - iTeam?
    float		flMaxPlayerSpeed;		// 0x012C
    float		flMaxPlayerSpeedAlt;	// 0x0130
    float		flSpread;				// 0x0134
    float		flSpreadAlt;			// 0x0138
    float		flInaccuracyCrouch;		// 0x013C
    float		flInaccuracyCrouchAlt;	// 0x0140
    float		flInaccuracyStand;		// 0x0144
    float		flInaccuracyStandAlt;	// 0x0148
    float		flInaccuracyJumpInitial;// 0x014C
    float		flInaccuracyJump;		// 0x0150
    float		flInaccuracyJumpAlt;	// 0x0154
    float		flInaccuracyLand;		// 0x0158
    float		flInaccuracyLandAlt;	// 0x015C
    float		flInaccuracyLadder;		// 0x0160
    float		flInaccuracyLadderAlt;	// 0x0164
    float		flInaccuracyFire;		// 0x0168
    float		flInaccuracyFireAlt;	// 0x016C
    float		flInaccuracyMove;		// 0x0170
    float		flInaccuracyMoveAlt;	// 0x0174
    float		flInaccuracyReload;		// 0x0178
    int			iRecoilSeed;			// 0x017C
    float		flRecoilAngle;			// 0x0180
    float		flRecoilAngleAlt;		// 0x0184
    float		flRecoilAngleVariance;	// 0x0188
    float		flRecoilAngleVarianceAlt;	// 0x018C
    float		flRecoilMagnitude;		// 0x0190
    float		flRecoilMagnitudeAlt;	// 0x0194
    float		flRecoilMagnitudeVariance;	// 0x0198
    float		flRecoilMagnitudeVarianceAlt;	// 0x019C
    float		flRecoveryTimeCrouch;	// 0x01A0
    float		flRecoveryTimeStand;	// 0x01A4
    float		flRecoveryTimeCrouchFinal;	// 0x01A8
    float		flRecoveryTimeStandFinal;	// 0x01AC
    int			iRecoveryTransitionStartBullet;// 0x01B0 
    int			iRecoveryTransitionEndBullet;	// 0x01B4
    bool		bUnzoomAfterShot;		// 0x01B8
    bool		bHideViewModelZoomed;	// 0x01B9
    char		pad_0x01B5[ 2 ];			// 0x01BA
    char		iZoomLevels[ 3 ];			// 0x01BC
    int			iZoomFOV[ 2 ];			// 0x01C0
    float		fZoomTime[ 3 ];			// 0x01C4
    char*		szWeaponClass;			// 0x01D4
    float		flAddonScale;			// 0x01D8
    char		pad_0x01DC[ 4 ];			// 0x01DC
    char*		szEjectBrassEffect;		// 0x01E0
    char*		szTracerEffect;			// 0x01E4
    int			iTracerFrequency;		// 0x01E8
    int			iTracerFrequencyAlt;	// 0x01EC
    char*		szMuzzleFlashEffect_1stPerson; // 0x01F0
    char		pad_0x01F4[ 4 ];			 // 0x01F4
    char*		szMuzzleFlashEffect_3rdPerson; // 0x01F8
    char		pad_0x01FC[ 4 ];			// 0x01FC
    char*		szMuzzleSmokeEffect;	// 0x0200
    float		flHeatPerShot;			// 0x0204
    char*		szZoomInSound;			// 0x0208
    char*		szZoomOutSound;			// 0x020C
    float		flInaccuracyPitchShift;	// 0x0210
    float		flInaccuracySoundThreshold;	// 0x0214
    float		flBotAudibleRange;		// 0x0218
    char		pad_0x0218[ 8 ];			// 0x0220
    char*		pWrongTeamMsg;			// 0x0224
    bool		bHasBurstMode;			// 0x0228
    char		pad_0x0225[ 3 ];			// 0x0229
    bool		bIsRevolver;			// 0x022C
    bool		bCannotShootUnderwater;	// 0x0230
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
		GetInaccuracyFn Inaccuracy = (GetInaccuracyFn)((*(PDWORD_PTR*)this)[ 469 ]); //459
		return Inaccuracy(this);
	}
	float GetFloatRecoilIndex()
	{
		return *reinterpret_cast<float*>((DWORD)this + 0x32D0);
	}

	float GetSpread()
	{
		typedef float(__thiscall* GetSpreadFn)(void*);
		GetSpreadFn Spread = (GetSpreadFn)((*(PDWORD_PTR*)this)[ 439 ]);//460
		return Spread(this);
	}

	void UpdateAccuracyPenalty()
	{
		typedef void(__thiscall* UpdateAccuracyPenaltyFn)(void*);
		UpdateAccuracyPenaltyFn Update = (UpdateAccuracyPenaltyFn)((*(PDWORD_PTR*)this)[ 470 ]);
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