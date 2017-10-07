#pragma once
#include "baseelement.h"
//#include "keygrabber.h"
//#include "selector.h"

//#include "dropdown.h"
//#include "playerlist.h"
//#include "colorpicker.h"

#include "window.h"
#include "category.h"

/*Mtab #include "tab.h"*/


#include "miniwindow.h"
#include "groupbox.h"
#include "dropdown.h"
#include "colorpicker.h"
#include "keybind.h"
#include "textbox.h"
#include "button.h"
#include "savepanel.h"
#include "Damagelist.h"
#include "checkbox.h"
#include "slider.h"
#include "tab.h"
#include "skinpanel.h"


class CMiscElements
{
public:
	MCategory* MiscCat;
	MGroupbox* Misc_General;
	MCheckbox* Misc_General_Restriction;
	MCheckbox* Misc_General_RevealRanks;
	MDropdown* Misc_General_Crosshair;
	MCheckbox* Misc_General_FOVON;
	MSlider* Misc_General_FOV;
	MCheckbox* Misc_General_Thirdperson;
	MDropdown* Misc_General_Thirdperson_Angles;
	MDropdown* Misc_General_AutoBuy_Main;
	MDropdown* Misc_General_AutoBuy_Secondary;

	MGroupbox* Misc_Movement;
	MCheckbox* Misc_Movement_Bunnyhop;
	MCheckbox* Misc_Movement_Autostrafe;
	MSlider* Misc_Movement_Strafe_Retrack;
	MDropdown* Misc_Movement_FakelagType;
	MSlider* Misc_Movement_Fakelag_Choke;
	MCheckbox* Misc_Movement_Fakelag_OnShoot;
	MCheckbox* Misc_Movement_Fakelag_OnGround;

	MGroupbox* Misc_Windows;
	MCheckbox* Misc_Windows_Playerlist;
	MCheckbox* Misc_Windows_Skinchanger;
	MCheckbox* Misc_Windows_Damagelist;
};
class CColorElements
{
public:
	MCategory* ColorCat;

	MGroupbox* Color_CT;

	
	MColorPicker* Color_CT_Visible;
	MColorPicker* Color_CT_Invisible;
	MColorPicker* Color_CT_Visible_Cham;
	MColorPicker* Color_CT_Invisible_Cham;

	MGroupbox* Color_T;
	MColorPicker* Color_T_Visible;
	MColorPicker* Color_T_Invisible;
	MColorPicker* Color_T_Visible_Cham;
	MColorPicker* Color_T_Invisible_Cham;

	MGroupbox* Color_Misc;
	MColorPicker* Color_Me_Visible;
	MColorPicker* Color_Me_Invisible;
	MColorPicker* Color_Me_Visible_Cham;
	MColorPicker* Color_Me_Invisible_Cham;
	MColorPicker* Color_Misc_Hands;
	MColorPicker* Color_Misc_Weapon;


	MGroupbox* Color_Entities;
	MColorPicker* Color_Weapon;
	MColorPicker* Color_Bomb;
	MColorPicker* Color_Planted_Bomb;
	MColorPicker* Color_Hostage;
	
};
class CMiniWindows
{
public: 
	MMiniWindow* Skinchanger = nullptr;
	MSkinchanger* Skinchanger_Panel = nullptr;
	MMiniWindow* Playerlist = nullptr;
	MMiniWindow* Damagelist = nullptr;
};
class CSaveElements
{
public:
	MCategory* SavePanelCat = nullptr;
	MGroupbox* Save_Box = nullptr;
	MSavePanel* SavePanel = nullptr;
};
class CVisualElements
{
public:
	MCategory* VisualCat;
	MGroupbox* Visual_Player;
	MCheckbox* Visual_Player_Active;
	MCheckbox* Visual_Player_EnemyOnly;
	MCheckbox* Visual_Player_Box;
	MCheckbox* Visual_Player_Bones;
	MCheckbox* Visual_Player_Health;
	MCheckbox* Visual_Player_Armor;
	MCheckbox* Visual_Player_DLight;
	MCheckbox* Visual_Player_Name;
	MCheckbox* Visual_Player_Weapon;
	MCheckbox* Visual_Player_Weapon_Ammo;
	MCheckbox* Visual_Player_Weapon_All;
	MCheckbox* Visual_Player_Rank;
	MCheckbox* Visual_Player_Wins;
	MCheckbox* Visual_Player_Glow;
	MCheckbox* Visual_Player_Snaplines;
	MCheckbox* Visual_Player_Grenade_Prediction;

	MGroupbox* Visual_CBaseEntity;
	MCheckbox* Visual_CBaseEntity_Active;
	MCheckbox* Visual_CBaseEntity_Name;
	MCheckbox* Visual_CBaseEntity_Box;
	MCheckbox* Visual_CBaseEntity_Glow;

	MGroupbox* Visual_CBaseEntity_Filter;
	MCheckbox* Visual_CBaseEntity_C4;
	MCheckbox* Visual_CBaseEntity_Weapons;
	MCheckbox* Visual_CBaseEntity_Hostages;
	MCheckbox* Visual_CBaseEntity_Grenades;

	MGroupbox* Visual_Chams;
	MCheckbox* Visual_Chams_Active;
	MDropdown* Visual_Chams_Style;
	MCheckbox* Visual_Chams_EnemyOnly;


	MGroupbox* Visual_Misc;
	MCheckbox* Visual_Misc_SelfESP;
	MDropdown* Visual_Misc_Hands;
	MDropdown* Visual_Misc_Weapon;
	MCheckbox* Visual_Misc_Nightmode;
	MDropdown* Visual_Misc_Skybox;
	MCheckbox* Visual_Misc_NoFlash;
	MCheckbox* Visual_Misc_NoSmoke;
	MCheckbox* Visual_Misc_NoScope;
	MCheckbox* Visual_Misc_NoParticles;
	MCheckbox* Visual_Misc_NoRecoil;
	MDropdown* Visual_Misc_Hitmaker;
};
class CRagebotElements
{
public:
	MCategory* RagebotCat = nullptr;
	MGroupbox* Ragebot_Main = nullptr;
	MCheckbox* Ragebot_Active = nullptr;
	MDropdown* Ragebot_Type = nullptr;
	MSlider* Ragebot_FOV = nullptr;
	MCheckbox* Ragebot_BodyAWP = nullptr;
	MCheckbox* Ragebot_AutoShoot = nullptr;

	MGroupbox* Ragebot_Selection = nullptr;
	MDropdown* Ragebot_SelectBy = nullptr;
	MCheckbox* Ragebot_AtFriendly = nullptr;
	MDropdown* Ragebot_Hitbox = nullptr;
	MDropdown* Ragebot_Hitscan = nullptr;
	MDropdown* Ragebot_Multipoint = nullptr;
	MSlider* Ragebot_Multipoint_Scale = nullptr;
	MCheckbox* Ragebot_Autowall = nullptr;
	MCheckbox* Ragebot_Autowall_Hitscan = nullptr;
	MSlider* Ragebot_MinDamage = nullptr;

	MGroupbox* Ragebot_Accuracy = nullptr;
	MCheckbox* Ragebot_NoRecoil = nullptr;
	MCheckbox* Ragebot_NoSpread = nullptr;
	MCheckbox* Ragebot_AutoScope = nullptr;
	MCheckbox* Ragebot_Hitchance = nullptr;
	MSlider * Ragebot_Hitchance_Slider = nullptr;
	MCheckbox* Ragebot_AutoStop = nullptr;


	MGroupbox* Ragebot_Corrections;
	MCheckbox* Ragebot_Corrections_PositionAdjustment = nullptr;
	MDropdown* Ragebot_Corrections_HeightAdjustment;
	MCheckbox* Ragebot_Corrections_Resolver;


	MGroupbox* Ragebot_AntiAim = nullptr;

	MCheckbox* Ragebot_AntiAim_Active;
	MDropdown* Ragebot_AntiAim_Pitch;
	MDropdown* Ragebot_AntiAim_Yaw;
	MDropdown* Ragebot_AntiAim_Yaw_Fake;
	MDropdown* Ragebot_AntiAim_Yaw_Real;
	MDropdown* Ragebot_AntiAim_AtTargets;
	MDropdown* Ragebot_AntiAim_Edge; 
	MCheckbox* Ragebot_AntiAim_Disable_Knife;
	MCheckbox* Ragebot_AntiAim_Disable_NoPlayers;

	MGroupbox* Ragebot_AntiAim_Custom = nullptr;
	MSlider* Ragebot_AntiAim_Custom_Fake_Yaw;
	MSlider* Ragebot_AntiAim_Custom_Real_Yaw;
	MSlider* Ragebot_AntiAim_Custom_Jitter_Range;
	MCheckbox* Ragebot_AntiAim_Custom_Rand_Jitter;
	MDropdown* Ragebot_AntiAim_Custom_Mode;
	MCheckbox* Ragebot_AntiAim_Custom_Dynamic;
};

