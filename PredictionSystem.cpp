#include "sdk.h"
#include "PredictionSystem.h"
#include "global.h"
#include "xor.h"
CPredictionSystem* g_PredictionSystem = new CPredictionSystem;
void CPredictionSystem::StartPrediction() {

	static bool bInit = false;
	if (!bInit) {
		m_pPredictionRandomSeed = *(int**)(FindPatternIDA(XorStr("client.dll"), XorStr("8B 0D ? ? ? ? BA ? ? ? ? E8 ? ? ? ? 83 C4 04")) + 2);
		bInit = true;
	}

	*m_pPredictionRandomSeed = G::UserCmd->random_seed;//MD5_PseudoRandom(G::UserCmd->command_number) & 0x7FFFFFFF;

	m_flOldCurtime = g_pGlobals->curtime;
	m_flOldFrametime = g_pGlobals->frametime;

	g_pGlobals->curtime = G::LocalPlayer->TickBase() * g_pGlobals->interval_per_tick;
	g_pGlobals->frametime = g_pGlobals->interval_per_tick;

	g_pGameMovement->StartTrackPredictionErrors(G::LocalPlayer);

	memset(&m_MoveData, 0, sizeof(m_MoveData));
	g_pMoveHelper->SetHost(G::LocalPlayer);
	g_pPrediction->SetupMove(G::LocalPlayer, G::UserCmd, g_pMoveHelper, &m_MoveData);
	g_pGameMovement->ProcessMovement(G::LocalPlayer, &m_MoveData);
	g_pPrediction->FinishMove(G::LocalPlayer, G::UserCmd, &m_MoveData);

}

void CPredictionSystem::EndPrediction() {

	g_pGameMovement->FinishTrackPredictionErrors(G::LocalPlayer);
	g_pMoveHelper->SetHost(0);

	*m_pPredictionRandomSeed = -1;

	g_pGlobals->curtime = m_flOldCurtime;
	g_pGlobals->frametime = m_flOldFrametime;
}