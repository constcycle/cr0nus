#include "hooks.h"
#include "GameUtils.h"
#include "Draw.h"
#include "Menu.h"
#include "Render.h"
#include "ESP.h"
#include "Misc.h"
#include "global.h"
#include "SpoofedConvar.h"
#include "Hitmarker.h"
bool first_frame_passed = false;
std::string sPanel = XorStr("FocusOverlayPanel");
void __fastcall Hooks::PaintTraverse(void* ecx/*thisptr*/, void* edx, unsigned int vgui_panel, bool force_repaint, bool allow_force)
{
		if (g_pEngine->IsConnected() && g_pEngine->IsInGame() && strstr(XorStr("HudZoom"), g_pPanel->GetName(vgui_panel)) && VisualElements.Visual_Misc_NoScope->Checked && G::LocalPlayer && G::LocalPlayer->IsScoped())
			return;

		panelVMT->GetOriginalMethod<PaintTraverseFn>(41)(ecx, vgui_panel, force_repaint, allow_force);

		const char* pszPanelName = g_pPanel->GetName(vgui_panel);


		if (!strstr(pszPanelName, sPanel.data()))
			return;

		

		int cur_height, cur_width; g_pEngine->GetScreenSize(cur_width, cur_height);

		if (!first_frame_passed || cur_width != G::Screen.width || cur_height != G::Screen.height)
		{
			first_frame_passed = true;
			g_Draw.Init();
//			g_Render.Initialise();
			g_pEngine->GetScreenSize(cur_width, cur_height);
			G::Screen.height = cur_height;
			G::Screen.width = cur_width;
		}

		
		if (g_pEngine->IsInGame() && g_pEngine->IsConnected() && G::LocalPlayer)
		{
			static auto ThirdPerson = false;
			if (MiscElements.Misc_General_Thirdperson->Checked)
			{

				//blockfaceit = true;

				auto svcheats = g_pCvar->FindVar("sv_cheats");
				auto svcheatsspoof = new SpoofedConvar(svcheats);
				svcheatsspoof->SetInt(1);

				if (G::LocalPlayer->GetHealth() > 0)
				{
					if (!ThirdPerson)
					{
						g_pEngine->ClientCmd("thirdperson");
						ThirdPerson = true;
					}
				}
				else
				{
					if (ThirdPerson)
					{
						g_pEngine->ClientCmd("firstperson");
						ThirdPerson = false;
					}
				}
			}
			else if (ThirdPerson)
			{
				ThirdPerson = false;
				g_pEngine->ClientCmd("firstperson");
			}

			if (VisualElements.Visual_Misc_NoParticles->Checked)
			{
				auto postprocessing = g_pCvar->FindVar("mat_postprocess_enable");
				auto postprocessingspoof = new SpoofedConvar(postprocessing);
				postprocessingspoof->SetInt(0);
				//bool& s_bOverridePostProcessingDisable = *(bool*)(FindPatternIDA("client.dll", "80 3D ? ? ? ? ? 53 56 57 0F 85") + 0x2);
				//s_bOverridePostProcessingDisable = true;
				/*
				InputPerSig = (CInput*) *(DWORD*)( Offsets->FindSignature( XOR( "client.dll" ), XOR( "B9 ? ? ? ? F3 0F 11 04 24 FF 50 10" ) ) + 1 );
GameRules = ( CGameRules* ) *( DWORD* )( Offsets->FindSignature( XOR( "client.dll" ), XOR( "A1 ? ? ? ? 85 C0 0F 84 ? ? ? ? 80 B8 ? ? ? ? ? 0F 84 ? ? ? ? 0F 10 05" ) ) + 1 );
MoveHelper = **reinterpret_cast< IMoveHelper*** >(Offsets->FindSignature( XOR( "client.dll" ), XOR( "8B 0D ? ? ? ? 8B 46 08 68" ) ) + 0x2);
Globals = **reinterpret_cast< CGlobalVarsBase*** >(( *reinterpret_cast< DWORD** >(BaseClientDLL) )[ 0 ] + 0x1B);
 
cOffset::isReady = FindSignature( XOR( "client.dll" ), XOR( "55 8B EC 83 E4 F8 83 EC 08 56 8B 35 ? ? ? ? 57 8B BE" ) );
 
cOffset::isBreakableCBaseEntity = FindSignature( XOR( "client.dll" ), XOR( "55 8B EC 51 56 8B F1 85 F6 74 68" ) );
 
cOffset::UTIL_ClipTraceToPlayers = FindSignature( XOR( "client.dll" ), XOR( "53 8B DC 83 EC 08 83 E4 F0 83 C4 04 55 8B 6B 04 89 6C 24 04 8B EC 81 EC ? ? ? ? 8B 43 10" ) );
 
cOffset::HandleBulletPenetration = FindSignature( XOR( "client.dll" ), XOR( "53 8B DC 83 EC 08 83 E4 F8 83 C4 04 55 8B 6B 04 89 6C 24 04 8B EC 83 EC 78 8B 53 14" ) );
 
cOffset::s_bOverridePostProcessing = FindSignature( XOR( "client.dll" ), XOR( "80 3D ? ? ? ? ? 53 56 57 0F 85" ) ) + 0x2;
 
cOffset::setClanTag = FindSignature( XOR( "engine.dll" ), XOR( "53 56 57 8B DA 8B F9 FF 15" ) );
 
cOffset::loadNamedSky = FindSignature( XOR( "engine.dll" ), XOR( "55 8B EC 81 EC ? ? ? ? 56 57 8B F9 C7 45" ) );
 
cOffset::m_pPredictionRandomSeed = FindSignature( XOR( "client.dll" ), XOR( "8B 0D ? ? ? ? BA ? ? ? ? E8 ? ? ? ? 83 C4 04" ) ) + 2;
 
Variables->ForceUpdate = (ForceUpdateFn)FindSignature( XOR( "engine.dll" ), XOR( "A1 ? ? ? ? B9 ? ? ? ? 56 FF 50 14 8B 34 85" ) );*/
			}

			//use that for endscene rendering -> bad tho, really bad xD
			//g_ESP->Update();
			g_ESP->Loop();
			//somewhy fixes endscene rendering
			g_Draw.Box(0, 0, 0, 0, 255, 255, 255, 255);

			/*lby anzeige*/
			if (G::LocalPlayer->GetHealth() > 0)
			{
				if (G::LocalPlayer->GetFlags() & FL_ONGROUND && G::LocalPlayer->GetVelocity().Length2D() > 0.1)
					G::lby_update_end_time = g_pGlobals->curtime;

				if (G::lby_update_end_time < g_pGlobals->curtime)
					g_Draw.StringA(g_Draw.font_build, false, 0, G::Screen.height - 80, 0, 255, 0, 255, "%s", "LBY");
				else
					g_Draw.StringA(g_Draw.font_build, false, 0, G::Screen.height - 80, 255, 0, 0, 255, "%s", "LBY");




			}
		}
}