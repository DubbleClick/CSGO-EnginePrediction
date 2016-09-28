#include "sdk.hpp"

void CPredictionSystem::StartPrediction() {

	m_flOldCurtime = g_pGlobals->curtime;
	m_flOldFrametime = g_pGlobals->frametime;

	g_pGlobals->curtime = g_pLocalPlayer->GetTickBase() * g_pGlobals->interval_per_tick;
	g_pGlobals->frametime = g_pGlobals->interval_per_tick;

	g_pGameMovement->StartTrackPredictionErrors(g_pLocalPlayer);

	memset(&m_MoveData, 0, sizeof(m_MoveData));
	g_pMoveHelper->SetHost(g_pLocalPlayer);
	g_pPrediction->SetupMove(g_pLocalPlayer, g_pUserCmd, g_pMoveHelper, &m_MoveData);
	g_pGameMovement->ProcessMovement(g_pLocalPlayer, &m_MoveData);
	g_pPrediction->FinishMove(g_pLocalPlayer, g_pUserCmd, &m_MoveData);

}

void CPredictionSystem::EndPrediction() {

	g_pGameMovement->FinishTrackPredictionErrors(g_pLocalPlayer);
	g_pMoveHelper->SetHost(0);

	g_pGlobals->curtime = m_flOldCurtime;
	g_pGlobals->frametime = m_flOldFrametime;
}
