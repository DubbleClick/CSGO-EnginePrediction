#pragma once

class CBaseEntity {
public:
	int GetTickBase() {
		return *(int*)((uintptr_t)this + 0x341C);
	}

	QAngle GetPunchAngles() {
		return *(QAngle*)((uintptr_t)this + 0x3020);
	}

	int GetShotsFired() {
		return *(int*)((uintptr_t)this + 0xA2B0);
	}
};

extern CBaseEntity* g_pLocalPlayer;