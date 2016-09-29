#pragma once
#include "sdk.hpp"

CreateMoveFn oCreateMove = nullptr;
bool __stdcall Hooks::CreateMove(float flInputSampleTime, CUserCmd* cmd) {
	bool ret = oCreateMove(flInputSampleTime, cmd);
	g_pLocalPlayer = g_pClientEntityList->GetClientEntity(g_pEngineClient->GetLocalPlayer());
	g_pUserCmd = cmd;

	static std::unique_ptr<CPredictionSystem> PredictionSystem = std::make_unique<CPredictionSystem>();
	static std::unique_ptr<CGlow>  Glow = std::make_unique<CGlow>();
	static std::unique_ptr<CRCS>  RCS = std::make_unique<CRCS>();
	static std::unique_ptr<CTrigger>  Trigger = std::make_unique<CTrigger>();
	static std::unique_ptr<CBunnyHop>  BHop = std::make_unique<CBunnyHop>();

	if (cmd->command_number == 0)
		return ret;

	Glow->Tick();
	BHop->Tick();

	PredictionSystem->StartPrediction();

	if (GetAsyncKeyState(VK_XBUTTON1))
		Trigger->Tick();

	if (GetAsyncKeyState(VK_XBUTTON2))
		RCS->Tick();

	PredictionSystem->EndPrediction();

	g_pUserCmd->viewangles.Clamp();

	return ret;
}