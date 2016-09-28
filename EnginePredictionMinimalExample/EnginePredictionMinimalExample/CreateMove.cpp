#pragma once
#include "sdk.hpp"

CreateMoveFn oCreateMove = nullptr;
bool __stdcall Hooks::CreateMove(float flInputSampleTime, CUserCmd* cmd) {
	bool ret = oCreateMove(flInputSampleTime, cmd);
	g_pLocalPlayer = g_pClientEntityList->GetClientEntity(g_pEngineClient->GetLocalPlayer());
	g_pUserCmd = cmd;

	static std::unique_ptr<CPredictionSystem> PredictionSystem = std::make_unique<CPredictionSystem>();

	if (cmd->command_number == 0)
		return ret;

	PredictionSystem->StartPrediction();

#pragma region shitrcs
	static QAngle m_oldPunch;
	static QAngle m_oldView = g_pUserCmd->viewangles;
	static bool bReset = false;
	if ((g_pUserCmd->buttons & IN_ATTACK) && g_pLocalPlayer->GetShotsFired() > 0) {
		QAngle m_curPunch = g_pLocalPlayer->GetPunchAngles() * 2.f;
		g_pUserCmd->viewangles += (m_oldPunch - m_curPunch);
		m_oldPunch = m_curPunch;
		bReset = true;
	}
	else {
		if (bReset) {
			g_pUserCmd->viewangles = m_oldView;
			bReset = false;
		}
		m_oldView = g_pUserCmd->viewangles;
		m_oldPunch.x = m_oldPunch.y = m_oldPunch.z = 0;
	}
#pragma endregion

	PredictionSystem->EndPrediction();

	g_pUserCmd->viewangles.Clamp();

	return ret;
}