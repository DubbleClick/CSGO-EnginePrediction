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

	QAngle GetOrigin() {
		return *(QAngle*)((uintptr_t)this + 0x134);
	}

	Vector GetEyePosition() {
		Vector origin = this->GetOrigin();
		Vector offset = *(Vector*)((uintptr_t)this + 0x104);

		return(origin + offset);
	}

	bool SetupBones(matrix3x4_t* pBoneToWorldOut, int nMaxBones, int boneMask, float currentTime = 0.f) {
		typedef bool(__thiscall* SetupBonesFn)(void*, matrix3x4_t*, int, int, float);
		return ((*(SetupBonesFn**)(void*)((uintptr_t)this + 0x4))[13])((void*)((uintptr_t)this + 0x4), pBoneToWorldOut, nMaxBones, boneMask, currentTime);
	}

	Vector GetBonePosition(int bone) {
		matrix3x4_t boneMatrixes[128];
		if (this->SetupBones(boneMatrixes, 128, 0x100, 0)) {
			matrix3x4_t boneMatrix = boneMatrixes[bone];
			return Vector(boneMatrix.flMatrix[0][3], boneMatrix.flMatrix[1][3], boneMatrix.flMatrix[2][3]);
		}
		return Vector(0, 0, 0);
	}
};

extern CBaseEntity* g_pLocalPlayer;