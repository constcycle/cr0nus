/*typedef struct player_info_s
{
char __pad1[0x10];
	char			name[32];
	int				userID;
	char			guid[33];
	unsigned long	friendsID;
	char			friendsName[32];
	bool			fakeplayer;
	bool			ishltv;
	unsigned long	customFiles[4];
	unsigned char	filesDownloaded;
	byte buffer[200];
} player_info_t;*/

/*class player_info_t {
public:
	char __pad1[0x10];
	char name[32];
	char __pad2[0x64];
	char guid[32 + 1];
	char __pad3[0x17B];
	bool fakeplayer;
};*/
typedef struct player_info_s {
private:
	DWORD __pad0[2];
public:
	int m_nXuidLow;
	int m_nXuidHigh;
	char m_szPlayerName[128];
	int m_nUserID;
	char m_szSteamID[33];
	UINT m_nSteam3ID;
	char m_szFriendsName[128];
	bool m_bIsFakePlayer;
	bool m_bIsHLTV;
	DWORD m_dwCustomFiles[4];
	BYTE m_FilesDownloaded;
private:
	int __pad1;
} player_info_t;