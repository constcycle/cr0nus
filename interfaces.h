extern CEngine *g_pEngine;
extern CClient *g_pClient;
extern CInput *g_pInput;
extern CClientEntityList *g_pEntitylist;
extern CModelInfo *g_pModelInfo;
extern CPanel* g_pPanel;
extern CSurface* g_pSurface;
extern CEngineTrace* g_pEngineTrace;
extern CClientModeShared* g_pClientMode;
extern CGlobalVarsBase* g_pGlobals;
extern CDebugOverlay* g_pDebugOverlay;
extern IPhysicsSurfaceProps* g_pPhysics;
extern CMaterialSystem* g_pMaterialSystem;
extern CModelRender* g_pModelRender;
extern CRenderView* g_pRenderView;
extern IMoveHelper* g_pMoveHelper;
extern CPrediction* g_pPrediction;
extern CInputSystem* g_pInputSystem;
extern CGlowObjectManager* g_pGlowObjectManager;
extern CGameMovement* g_pGameMovement;
extern IGameEventManager* g_pGameEventManager;
extern ICvar* g_pCvar;
extern CEffects* g_pEffects;
extern IEngineVGui* g_pEngineVGUI;
extern C_TEFireBullets* g_pFireBullet;
#include "Utilities.h"


class CInterfaces
{
public:
	
	bool GetInterfaces()
	{
		g_pEngine = (CEngine*)GetPointer(XorStr("engine.dll"), XorStr("VEngineClient"));
		g_pEngineTrace = (CEngineTrace*)GetPointer(XorStr("engine.dll"), XorStr("EngineTraceClient"));
		g_pClient = (CClient*)GetPointer(XorStr("client.dll"), XorStr("VClient"));
		g_pEntitylist = (CClientEntityList*)GetPointer(XorStr("client.dll"), XorStr("VClientEntityList"));
		g_pModelInfo = (CModelInfo*)GetPointer(XorStr("engine.dll"), XorStr("VModelInfoClient"));
		g_pPanel = (CPanel*)GetPointer(XorStr("vgui2.dll"), XorStr("VGUI_Panel"));
		g_pSurface = (CSurface*)GetPointer(XorStr("vguimatsurface.dll"), XorStr("VGUI_Surface"));
		g_pDebugOverlay = (CDebugOverlay*)GetPointer(XorStr("engine.dll"), XorStr("VDebugOverlay"));
		g_pPhysics = (IPhysicsSurfaceProps*)GetPointer(XorStr("vphysics.dll"), XorStr("VPhysicsSurfaceProps"));
		g_pMaterialSystem = (CMaterialSystem*)GetPointer(XorStr("materialsystem.dll"), XorStr("VMaterialSystem"));
		g_pModelRender = (CModelRender*)GetPointer(XorStr("engine.dll"), XorStr("VEngineModel"));
		g_pRenderView = (CRenderView*)GetPointer(XorStr("engine.dll"), XorStr("VEngineRenderView"));
		g_pGameMovement = (CGameMovement*)GetPointer(XorStr("client.dll"), XorStr("GameMovement"));
		g_pPrediction = (CPrediction*)GetPointer(XorStr("client.dll"), XorStr("VClientPrediction"));
		g_pGameEventManager = (IGameEventManager*)GetPointer2(XorStr("engine.dll"), XorStr("GAMEEVENTSMANAGER002"));
		g_pCvar = (ICvar*)GetPointer(XorStr("vstdlib.dll"), XorStr("VEngineCvar"));
		g_pInputSystem = (CInputSystem*)GetPointer(XorStr("inputsystem.dll"), XorStr("InputSystemVersion"));
		g_pEffects = (CEffects*)GetPointer(XorStr("engine.dll"), XorStr("VEngineEffects"));
		g_pEngineVGUI = (IEngineVGui*)GetPointer(XorStr("engine.dll"), XorStr("VEngineVGui"));
		g_pInput = **(CInput***)(FindPatternIDA("client.dll", "B9 ? ? ? ? F3 0F 11 04 24 FF 50 10") + 1);//((*(uintptr_t**)g_pClient)[15] + 0x1);
		PDWORD pdwClient = (PDWORD)*(PDWORD)g_pClient;
		g_pGlobals = **(CGlobalVarsBase***)((*(uintptr_t**)g_pClient)[0] + 0x1B);//(CGlobalVarsBase*)*(PDWORD)*(PDWORD)(Utilities::Memory::FindPattern(XorStr("client.dll"), (PBYTE)XorStr("\xA1\x00\x00\x00\x00\x8B\x40\x10\x89\x41\x04"), XorStr("x????xxxxxx")) + 0x1);
		g_pClientMode = **(CClientModeShared***)((*(uintptr_t**)g_pClient)[10] + 0x5);
		g_pMoveHelper = **(IMoveHelper***)(FindPatternIDA(XorStr("client.dll"), XorStr("8B 0D ? ? ? ? 8B 46 08 68")) + 0x2);
		
		g_pFireBullet = (C_TEFireBullets*)((DWORD)GetModuleHandle("client.dll") + 0xA5B498);//(FindPatternIDA("client.dll", "8B D1 B8 ? ? ? ? B9") + 0x81);
		
	
		printf(XorStr("FireBullet Found: 0x%X\n"), (DWORD)g_pFireBullet);
		printf(XorStr("IGlobals Found: 0x%X\n"), (DWORD)g_pGlobals);
		printf(XorStr("ClientMode Found: 0x%X\n"), (DWORD)g_pClientMode);
		printf(XorStr("MoveHelper Found: 0x%X\n"), (DWORD)g_pMoveHelper);

		

		


		
		g_pGlowObjectManager = (CGlowObjectManager*)*(DWORD*)(FindPatternIDA("client.dll", "A1 ? ? ? ? A8 01 75 4B") + 0x19);
		printf(XorStr("GlowObjectManager Found: 0x%X\n"), (DWORD)g_pGlowObjectManager);
		return true;
	}

	void* GetPointer(const char* Module, const char* InterfaceName)
	{
		void* Interface = NULL;
		char PossibleInterfaceName[1024];

		CreateInterfaceFn CreateInterface = (CreateInterfaceFn)GetProcAddress(GetModuleHandleA(Module), XorStr("CreateInterface"));

			for (int i = 1; i < 100; i++)
			{
				sprintf_s(PossibleInterfaceName, XorStr("%s0%i"), InterfaceName, i);
				Interface = (void*)CreateInterface(PossibleInterfaceName, NULL);
				if (Interface != NULL)
				{
					printf(XorStr("%s Found: 0x%X\n"), PossibleInterfaceName, (DWORD)Interface);
					break;
				}
				sprintf_s(PossibleInterfaceName, XorStr("%s00%i"), InterfaceName, i);
				Interface = (void*)CreateInterface(PossibleInterfaceName, NULL);
				if (Interface != NULL)
				{
					printf(XorStr("%s Found: 0x%X\n"), PossibleInterfaceName, (DWORD)Interface);
					break;
				}
			}
		

		return Interface;
	}
	void *GetPointer2(const char* Module, const char* InterfaceName)
	{
		void* Interface = NULL;
		

		CreateInterfaceFn CreateInterface = (CreateInterfaceFn)GetProcAddress(GetModuleHandleA(Module), XorStr("CreateInterface"));
		Interface = (void*)CreateInterface(InterfaceName, NULL);
		if (Interface != NULL)
		{
			//FUUUUUUUUUUUUCK
		}
		return Interface;
	}
}; extern CInterfaces Interfaces;