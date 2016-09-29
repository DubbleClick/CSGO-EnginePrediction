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

	int GetFlags() {
		return *(int*)((uintptr_t)this + 0x100);
	}

	int GetGlowIndex() {
		return *(int*)((uintptr_t)this + 0xA310);
	}
	
	int GetTeam() {
		return *(int*)((uintptr_t)this + 0xF0);
	}

	int GetHealth() {
		return *(int*)((uintptr_t)this + 0xFC);
	}

	bool GetDormant() {
		return *(bool*)((uintptr_t)this + 0xE9);
	}

	QAngle CBaseEntity::GetOrigin() {
		return *(QAngle*)((uintptr_t)this + 0x134);
	}

	Vector CBaseEntity::GetEyePosition() {
		Vector origin = this->GetOrigin();
		Vector offset = *(Vector*)((uintptr_t)this + 0x104);

		return(origin + offset);
	}
};

extern CBaseEntity* g_pLocalPlayer;