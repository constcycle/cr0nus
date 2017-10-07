#pragma once
#include "sdk.h"

class stackframe {
private:
	void* base = nullptr;
public:
	stackframe(void* base) {
		this->base = base;
	}

	void previous(unsigned int frames = 1) {
		for (; frames < 0; --frames) {
			base = *(void**)base;
		}
	}

	template <typename t>
	t get_local(uintptr_t offset) {
		(t)((uintptr_t)base - offset);
	}

	template <typename t>
	t get_arg(uintptr_t offset) {
		(t)get_retaddr() + offset;
	}

	uintptr_t get_retaddr() {
		return (uintptr_t)base + sizeof(uintptr_t);
	}
};

extern VMT* panelVMT;
extern VMT* clientmodeVMT;
extern VMT* enginevguiVMT;
extern VMT* modelrenderVMT;
extern VMT* clientVMT;
extern VMT* firebulletVMT;
extern VMT* enginetraceVMT;
extern VMT* d3d9VMT;

typedef void(__stdcall *FrameStageNotifyFn) (ClientFrameStage_t);
typedef void(__thiscall *PaintTraverseFn) (void*, /*VPANEL*/unsigned int, bool, bool);
typedef void(__thiscall *DrawModelExecuteFn) (void*, void*,void*,  const ModelRenderInfo_t&, matrix3x4_t*);
typedef bool(__thiscall *CreateMoveFn) (void*, float, CUserCmd*);
//typedef void(__stdcall* CreateMoveFn)(int, float, bool);
//typedef long(__stdcall* EndSceneFn)(IDirect3DDevice9* device);
typedef long(__stdcall* EndSceneFn)(IDirect3DDevice9* device);
typedef void(__thiscall* TE_FireBullets_PostDataUpdateFn)(C_TEFireBullets*, DataUpdateType_t);
typedef void(__thiscall *TraceRayFn)(void*, const Ray_t &ray, unsigned int fMask, ITraceFilter *pTraceFilter, trace_t *pTrace);
//typedef bool(*FireEventFn) (void*, IGameEvent*, bool);
//typedef bool(*FireEventClientSideFn) (void*, IGameEvent*);
//typedef int(*IN_KeyEventFn) (void*, int, int, const char*);
//typedef void(*RenderViewFn) (void*, CViewSetup&, CViewSetup&, unsigned int, int);
//typedef void(*SetKeyCodeStateFn) (void*, ButtonCode_t, bool);
//typedef void(*SetMouseCodeStateFn) (void*, ButtonCode_t, MouseCodeState_t);
//typedef void(*OnScreenSizeChangedFn) (void*, int, int);
//typedef void(*PlaySoundFn) (void*, const char*);
//typedef void(*BeginFrameFn) (void*, float);
//typedef int(*PumpWindowsMessageLoopFn) (void*, void*);
typedef void(__stdcall *PaintFn) (PaintMode_t);
//typedef void(*EmitSound1Fn) (void*, IRecipientFilter&, int, int, const char*, unsigned int, const char*, float, int, float, int, int, const Vector*, const Vector*, void*, bool, float, int);
//typedef void(*EmitSound2Fn) (void*, IRecipientFilter&, int, int, const char*, unsigned int, const char*, float, int, soundlevel_t, int, int, const Vector*, const Vector*, void*, bool, float, int);
typedef void(*RenderSmokePostViewmodelFn) (void*);
typedef void(__thiscall *OverrideViewFn) (void*, CViewSetup*);
typedef float(*GetViewModelFOVFn) (void*);

typedef void(*StartDrawingFn) (void*);
typedef void(*FinishDrawingFn) (void*);


namespace Hooks
{
	void __fastcall PaintTraverse(void* ecx/*thisptr*/, void* edx, unsigned int vgui_panel, bool force_repaint, bool allow_force);
	void __stdcall FrameStageNotify(ClientFrameStage_t stage);
	void __fastcall DrawModelExecute(void* ecx, void* edx, void* * ctx, void *state, const ModelRenderInfo_t &pInfo, matrix3x4_t *pCustomBoneToWorld);
	bool __fastcall CreateMove(void* thisptr, void*, float SampleTime, CUserCmd* pCmd);
	//void __stdcall CreateMoveProxy(int sequence_number, float input_sample_frameTime, bool active, bool& bSendPacket);
	void __fastcall/*__stdcall*/ FireBullets_PostDataUpdate(C_TEFireBullets* thisptr, DataUpdateType_t updateType);
	//bool FireEvent(void* thisptr, IGameEvent* event, bool bDontBroadcast);
	//bool FireEventClientSide(void* thisptr, IGameEvent* event);
	//int IN_KeyEvent(void* thisptr, int eventcode, int keynum, const char* currentbinding);
	//void RenderView(void* thisptr, CViewSetup& setup, CViewSetup& hudViewSetup, unsigned int nClearFlags, int whatToDraw);
	void __fastcall OverrideView(void* _this, void* _edx, CViewSetup* setup);
	void __fastcall TraceRay(void *thisptr, void*, const Ray_t &ray, unsigned int fMask, ITraceFilter *pTraceFilter, trace_t *pTrace);
	//void SetKeyCodeState(void* thisptr, ButtonCode_t code, bool bPressed);
	//void SetMouseCodeState(void* thisptr, ButtonCode_t code, MouseCodeState_t state);
	//void OnScreenSizeChanged(void* thisptr, int oldwidth, int oldheight);
	//void PlaySound(void* thisptr, const char* filename);
	//void BeginFrame(void* thisptr, float frameTime);
	//int PumpWindowsMessageLoop(void* thisptr, void* unknown);
	void __stdcall Paint(PaintMode_t mode);
	HRESULT __stdcall D3D9_EndScene(IDirect3DDevice9* pDevice);
	//void EmitSound1(void* thisptr, IRecipientFilter& filter, int iEntIndex, int iChannel, const char* pSoundEntry, unsigned int nSoundEntryHash, const char *pSample, float flVolume, int nSeed, float flAttenuation, int iFlags, int iPitch, const Vector* pOrigin, const Vector* pDirection, void* pUtlVecOrigins, bool bUpdatePositions, float soundtime, int speakerentity);
	//void EmitSound2(void* thisptr, IRecipientFilter& filter, int iEntIndex, int iChannel, const char* pSoundEntry, unsigned int nSoundEntryHash, const char *pSample, float flVolume, int nSeed, soundlevel_t iSoundLevel, int iFlags, int iPitch, const Vector* pOrigin, const Vector* pDirection, void* pUtlVecOrigins, bool bUpdatePositions, float soundtime, int speakerentity);
	void RenderSmokePostViewmodel(void* thisptr);
	float GetViewModelFOV(void* thisptr);



	extern WNDPROC                            g_pOldWindowProc; //Old WNDPROC pointer
	extern HWND                               g_hWindow; //Handle to the CSGO window
	LRESULT   __stdcall WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
}