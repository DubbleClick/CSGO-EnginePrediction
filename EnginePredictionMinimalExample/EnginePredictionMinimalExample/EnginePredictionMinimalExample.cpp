#include "sdk.hpp"
#include "vmt.h"
#include <thread>

CUserCmd* g_pUserCmd;
CGlobalVarsBase* g_pGlobals;
CBaseEntity* g_pLocalPlayer;
IClientEntityList* g_pClientEntityList;
IEngineClient* g_pEngineClient;
IGameMovement* g_pGameMovement;
IMoveHelper* g_pMoveHelper;
IPrediction* g_pPrediction;

std::unique_ptr<VMT::VMTManager> ClientModeVMTManager;
HMODULE DllModule;

BOOL WINAPI Initialize() {
	uintptr_t* Client = Util::CreateInterface<uintptr_t*>("client.dll", "VClient017");
	uintptr_t* ClientMode = **(uintptr_t***)((*(uintptr_t**)Client)[10] + 0x5);
	g_pGlobals = **(CGlobalVarsBase***)((*(uintptr_t**)Client)[0] + 0x53);
	g_pClientEntityList = Util::CreateInterface<IClientEntityList*>("client.dll", "VClientEntityList003");
	g_pEngineClient = Util::CreateInterface<IEngineClient*>("engine.dll", "VEngineClient014");
	g_pGameMovement = Util::CreateInterface<IGameMovement*>("client.dll", "GameMovement001");
	g_pPrediction = Util::CreateInterface<IPrediction*>("client.dll", "VClientPrediction001");
	g_pMoveHelper = **(IMoveHelper***)(Util::FindPattern("client.dll", "8B 0D ? ? ? ? 8B 46 08 68") + 2);

	ClientModeVMTManager = std::make_unique<VMT::VMTManager>(ClientMode);
	ClientModeVMTManager->HookMethod(Hooks::CreateMove, 24);
	oCreateMove = ClientModeVMTManager->GetMethod<CreateMoveFn>(24);

	while (!GetAsyncKeyState(VK_END))
		std::this_thread::sleep_for(std::chrono::milliseconds(10));

	FreeLibraryAndExitThread(DllModule, EXIT_SUCCESS);

	return TRUE;
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved) {
	switch (ul_reason_for_call) {
		case DLL_PROCESS_ATTACH:
			DllModule = hModule;
			CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)Initialize, NULL, 0, NULL);
			break;
		case DLL_THREAD_ATTACH:
		case DLL_THREAD_DETACH:
			break;
		case DLL_PROCESS_DETACH:
			ClientModeVMTManager->Kill();
			break;
	}
	return TRUE;
}

