#pragma once
#include "sdk.hpp"

CreateMoveFn oCreateMove = nullptr;
bool __stdcall Hooks::CreateMove(float flInputSampleTime, CUserCmd* cmd) {
	bool ret = oCreateMove(flInputSampleTime, cmd);
	g_pLocalPlayer = g_pClientEntityList->GetClientEntity(g_pEngineClient->GetLocalPlayer());
	g_pUserCmd = cmd;

	static CPredictionSystem PredictionSystem;
	static CGlow  Glow;
	static CRCS RCS;
	static CTrigger Trigger;
	static CBunnyHop Bhop;
	static CAimbot Aimbot;

	if (cmd->command_number == 0)
		return ret;

	Glow.Tick();
	Bhop.Tick();

	PredictionSystem.StartPrediction();

	if (GetAsyncKeyState(VK_XBUTTON1))
		Trigger.Tick();

	if (GetAsyncKeyState(VK_XBUTTON2))
		Aimbot.Tick();
	else if (GetAsyncKeyState(VK_MBUTTON))
		RCS.Tick();

	PredictionSystem.EndPrediction();

	g_pUserCmd->viewangles.Clamp();

	return ret;
}