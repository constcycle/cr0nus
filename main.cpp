#include "sdk.h"
#include "hooks.h"
#include "Menu.h"
#include "skinsutil.h"
#include "MaterialHelper.h"
#include "BacktrackingHelper.h"
#include "security.h"
#include "Math.h"
#include "post.h"
#include "global.h"
#include <dbghelp.h>
#include <tchar.h>
#include "skinchanger.h"

HINSTANCE hAppInstance;
CClient* g_pClient;
CClientEntityList* g_pEntitylist;
CEngine* g_pEngine;
CInput* g_pInput;
CModelInfo* g_pModelInfo;
CInputSystem* g_pInputSystem;
CPanel* g_pPanel;
CSurface* g_pSurface;
CEngineTrace* g_pEngineTrace;
CDebugOverlay* g_pDebugOverlay;
IPhysicsSurfaceProps* g_pPhysics;
CRenderView* g_pRenderView;
CClientModeShared* g_pClientMode;
CGlobalVarsBase* g_pGlobals;
CModelRender* g_pModelRender;
CMaterialSystem* g_pMaterialSystem;
IMoveHelper* g_pMoveHelper;
CGlowObjectManager* g_pGlowObjectManager;
CPrediction* g_pPrediction;
CGameMovement* g_pGameMovement;
IGameEventManager* g_pGameEventManager;
IEngineVGui* g_pEngineVGUI;
ICvar* g_pCvar;
CEffects* g_pEffects;
CInterfaces Interfaces;
CCRC gCRC;
C_TEFireBullets* g_pFireBullet;

VMT* panelVMT;
VMT* clientmodeVMT;
VMT* enginevguiVMT;
VMT* modelrenderVMT;
VMT* clientVMT;
VMT* firebulletVMT;
VMT* d3d9VMT;
VMT* enginetraceVMT;

HMODULE h_ThisModule;
void FixX(const CRecvProxyData *pData, void *pStruct, void *pOut)
{
	float flPitch = Math::NormalizeYaw(pData->m_Value.m_Float);

	CBaseEntity* ent = (CBaseEntity*)pStruct;

	int index = ent->Index();

	g_BacktrackHelper->PlayerRecord[index].EyeAngles.x = flPitch;

	*(float*)(pOut) = flPitch;
}

void FixY(const CRecvProxyData *pData, void *pStruct, void *pOut)
{
	float flYaw = Math::NormalizeYaw(pData->m_Value.m_Float);

	CBaseEntity* ent = (CBaseEntity*)pStruct;

	int index = ent->Index();
	
	g_BacktrackHelper->PlayerRecord[index].EyeAngles.y = flYaw;

	*(float*)(pOut) = flYaw;
}
void LBY_Proxy(const CRecvProxyData *pData, void *pStruct, void *pOut)
{
	float flLby = Math::NormalizeYaw(pData->m_Value.m_Float);

	CBaseEntity* ent = (CBaseEntity*)pStruct;

	if (!ent->GetVelocity().Length() && ent->GetIndex() != g_pEngine->GetLocalPlayer())
	{
		for (int i = 0; i < g_BacktrackHelper->PlayerRecord[ent->GetIndex()].records.size(); i++)
		{
			if (ent->GetSimulationTime() == g_BacktrackHelper->PlayerRecord[ent->GetIndex()].records.at(i).m_flSimulationTime)
				g_BacktrackHelper->PlayerRecord[ent->GetIndex()].records.at(i).bLowerBodyYawUpdated = true;
		}
	}

	if (ent->GetIndex() == g_pEngine->GetLocalPlayer())
		G::lby_update_end_time = g_pGlobals->curtime + 0.2f;

	*(float*)(pOut) = flLby;
}
void HookProxies()
{
	ClientClass *pClass = g_pClient->GetAllClasses();
	while (pClass)
	{
		const char *pszName = pClass->m_pRecvTable->m_pNetTableName;
		if (!strcmp(pszName, XorStr("DT_BaseViewModel")))
		{
			for (int i = 0; i < pClass->m_pRecvTable->m_nProps; i++)
			{
				RecvProp *pProp = &(pClass->m_pRecvTable->m_pProps[i]);

				if (!strcmp(pProp->m_pVarName, XorStr("m_nModelIndex")))
				{
					//oModelIndexProxy = (RecvVarProxyFn)pProp->m_ProxyFn;
					pProp->m_ProxyFn = ViewModel_Index_Proxy;
				}
				if (!strcmp(pProp->m_pVarName, XorStr("m_nSequence")))
				{
					//oSequenceProxy = (RecvVarProxyFn)pProp->m_ProxyFn;
					//pProp->m_ProxyFn = RecvProxy_Sequence;
				}
			}
		}
		if (!strcmp(pszName, XorStr("DT_CSPlayer")))
		{
			for (int i = 0; i < pClass->m_pRecvTable->m_nProps; i++)
			{
				RecvProp *pProp = &(pClass->m_pRecvTable->m_pProps[i]);
				const char *name = pProp->m_pVarName;

				// Pitch Fix
				if (!strcmp(name, XorStr("m_angEyeAngles[0]")))
				{
					//oPitchProxy = (RecvVarProxyFn)pProp->m_ProxyFn;
					pProp->m_ProxyFn = FixX;
					//printf("Pitch Proxy Hooked\n");
				}

				// Yaw Fix
				if (!strcmp(name, XorStr("m_angEyeAngles[1]")))
				{
					//oYawProxy = (RecvVarProxyFn)pProp->m_ProxyFn;
					//printf("Yaw Proxy Hooked!\n");
					pProp->m_ProxyFn = FixY;
				}
				if (!strcmp(name, XorStr("m_flLowerBodyYawTarget")))
				{
					pProp->m_ProxyFn = LBY_Proxy;
				}
			}
		}


		pClass = pClass->m_pNext;
	}
}
#include <Shlwapi.h>
#pragma comment(lib, "shlwapi.lib")
TCHAR* GetThisPath(TCHAR* dest, size_t destSize)
{
	if (!dest) 
		return NULL;
	if (MAX_PATH > destSize)
		return NULL;

	DWORD length = GetModuleFileName(NULL, dest, destSize);
	PathRemoveFileSpec(dest);
	return dest;
}
LONG WINAPI UnhandledExFilter(PEXCEPTION_POINTERS ExPtr)
{
	BOOL(WINAPI* pMiniDumpWriteDump)(IN HANDLE hProcess, IN DWORD ProcessId, IN HANDLE hFile, IN MINIDUMP_TYPE DumpType, IN CONST PMINIDUMP_EXCEPTION_INFORMATION ExceptionParam, OPTIONAL IN CONST PMINIDUMP_USER_STREAM_INFORMATION UserStreamParam, OPTIONAL IN CONST PMINIDUMP_CALLBACK_INFORMATION CallbackParam OPTIONAL) = NULL;

	HMODULE hLib = LoadLibrary(_T("dbghelp"));
	if (hLib)
		*(void**)&pMiniDumpWriteDump = (void*)GetProcAddress(hLib, "MiniDumpWriteDump");

	TCHAR buf[MAX_PATH], buf2[MAX_PATH];

	if (pMiniDumpWriteDump) 
	{
		
		GetThisPath(buf, sizeof(buf));

		int rnd;
		__asm push edx
		__asm rdtsc
		__asm pop edx
		__asm mov rnd, eax
		rnd &= 0xFFFF;
		wsprintf(buf2, _T("%s\\Crash_%x.dmp"), buf, rnd);
		HANDLE hFile = CreateFile(buf2, GENERIC_READ | GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

		if (hFile != INVALID_HANDLE_VALUE) {
			MINIDUMP_EXCEPTION_INFORMATION md;
			md.ThreadId = GetCurrentThreadId();
			md.ExceptionPointers = ExPtr;
			md.ClientPointers = FALSE;
			BOOL win = pMiniDumpWriteDump(GetCurrentProcess(), GetCurrentProcessId(), hFile, MiniDumpNormal, &md, 0, 0);

			if (!win)
				wsprintf(buf, _T("MiniDumpWriteDump failed. Error: %u \n(%s)"), GetLastError(), buf2);
			else
				wsprintf(buf, _T("Minidump created:\n%s"), buf2);
			CloseHandle(hFile);

		}
		else {
			wsprintf(buf, _T("Could not create minidump:\n%s"), buf2);
		}
	}
	else {
		wsprintf(buf, _T("Could not load dbghelp"));
	}
	MessageBox(NULL, buf, _T("Unhandled exception"), MB_OK | MB_ICONERROR);
	ExitProcess(0);    //do whatever u want here
}


void Init()
{	
//	SkinsUtil::ParseSkins();
	SetupOffsets();
	HookProxies();
	g_MaterialHelper = new CMaterialHelper();
	g_Event.RegisterSelf();
	srand(time(0));

	panelVMT = new VMT(g_pPanel);
	panelVMT->HookVM((void*)Hooks::PaintTraverse, 41);
	panelVMT->ApplyVMT();

	clientmodeVMT = new VMT(g_pClientMode);
	clientmodeVMT->HookVM((void*)Hooks::CreateMove, 24);
	clientmodeVMT->HookVM((void*)Hooks::OverrideView, 18);
	clientmodeVMT->ApplyVMT();

	modelrenderVMT = new VMT(g_pModelRender);
	modelrenderVMT->HookVM((void*)Hooks::DrawModelExecute, 21);
	modelrenderVMT->ApplyVMT();

	clientVMT = new VMT(g_pClient);
	clientVMT->HookVM((void*)Hooks::FrameStageNotify, 36);
	clientVMT->ApplyVMT();



	auto dwDevice = **(uint32_t**)(FindPatternIDA(XorStr("shaderapidx9.dll"), XorStr("A1 ? ? ? ? 50 8B 08 FF 51 0C")) + 1);
	d3d9VMT = new VMT((void*)dwDevice);
	d3d9VMT->HookVM((void*)Hooks::D3D9_EndScene, 42);
	d3d9VMT->ApplyVMT();

	

	while (!(Hooks::g_hWindow = FindWindowA(XorStr("Valve001"), NULL))) 
		Sleep(200);

	if (Hooks::g_hWindow)
		Hooks::g_pOldWindowProc = (WNDPROC)SetWindowLongPtr(Hooks::g_hWindow, GWLP_WNDPROC, (LONG_PTR)Hooks::WndProc);
	
}
void StartCheat()
{
	//Utilities::OpenConsole("blyat");
	if (Interfaces.GetInterfaces() && g_pPanel && g_pClientMode)
	{
	//	Msg("Engine Build: %i\n", g_pEngine->GetEngineBuildNumber());
	//	if (g_pEngine->GetEngineBuildNumber() != 13591)
	//	{
	//		MessageBox(NULL, "Please visit the forum for further infos!", "Update-Error", 0);
		//	FreeLibraryAndExitThread(hAppInstance, 0x0);
		//	return;
	//	}
		Sleep(500);
		Init();
	}
}

void Nothing(HINSTANCE Instance)
{
	while (!GetAsyncKeyState(VK_END))
	{
		Sleep(150);
	}
	Utilities::CloseConsole();
	
	FreeLibraryAndExitThread(Instance, 0x0);
}
static HMODULE DllBaseAddress() // works with manually mapped dlls (will get us our HMODULE)
{
	MEMORY_BASIC_INFORMATION info;
	size_t len = VirtualQueryEx(GetCurrentProcess(), (void*)DllBaseAddress, &info, sizeof(info));
	Assert(len == sizeof(info));
	return len ? (HMODULE)info.AllocationBase : NULL;
}

BOOL WINAPI DllMain(HINSTANCE Instance, DWORD dwReason, LPVOID lpReserved)
{
	switch (dwReason)
	{


	case DLL_PROCESS_ATTACH:
				CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)StartCheat, NULL, NULL, NULL);
			//	CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)Nothing, NULL, NULL, NULL);
	
			break;
}
	return true;
}
MsgFn oMsg;
void __cdecl Msg(char const* msg, ...)
{
	//DOES NOT CRASH
	if(!oMsg)
		oMsg = (MsgFn)GetProcAddress(GetModuleHandle(XorStr("tier0.dll")), XorStr("Msg"));

	char buffer[989];
	va_list list;
	va_start(list, msg);
	vsprintf_s(buffer, msg, list);
	va_end(list);
	oMsg(buffer, list);
}