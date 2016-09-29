#pragma once

#define IN_ATTACK (1 << 0)
#define IN_JUMP (1 << 1)
#define FL_ONGROUND (1 << 0)

extern uintptr_t g_dwGlowObject;

class Color {
public:
	unsigned char r, g, b, a;

	Color(unsigned char w, unsigned char x, unsigned char y, unsigned char z = 255) {
		r = w;
		g = x;
		b = y;
		a = z;
	}

	Vector ToVector() {
		return Vector((float)r / 255.f, (float)g / 255.f, (float)b / 255.f);
	}

	float GetAlphaFloat() {
		return ((float)a / 255.f);
	}
};

class CBunnyHop {
public: void Tick();
};

class CGlow {
private:
	class GlowStruct_t {
	public:
		CBaseEntity*		m_hEntity;
		Vector				m_vGlowColor;
		float				m_flGlowAlpha;

		char				unknown[4];
		float				flUnk;
		float				m_flBloomAmount;
		float				localplayeriszeropoint3;

		bool				m_bRenderWhenOccluded;
		bool				m_bRenderWhenUnoccluded;
		bool				m_bFullBloomRender;
		char				unknown1[1];

		int					m_nFullBloomStencilTestValue;
		int					iUnk;
		int					m_nSplitScreenSlot;
		int					m_nNextFreeSlot;

		static const int END_OF_FREE_LIST = -1;
		static const int ENTRY_IN_USE = -2;
	};

	void GlowEntity(int index, Color clr);
public:
	void Tick();
};

class CRCS {
public:
	void Tick();

private:
	QAngle m_oldPunch;
	QAngle m_oldView;
	bool bReset = false;
};

class CTrigger {
public: void Tick();
};

//function defs

inline void CBunnyHop::Tick() {
	if (g_pUserCmd->buttons & IN_JUMP) {
		if (!(g_pLocalPlayer->GetFlags() & FL_ONGROUND))
			g_pUserCmd->buttons &= ~IN_JUMP;
	}
}

inline void CTrigger::Tick() {
	Vector src, dst, forward;
	trace_t tr;
	Ray_t ray;
	ITraceFilter filter;
	filter.pSkip = g_pLocalPlayer;

	QAngle viewangle = g_pUserCmd->viewangles;

	viewangle += g_pLocalPlayer->GetPunchAngles() * 2.f;

	Math::AngleVectors(viewangle, forward);
	forward *= 8012.f;
	filter.pSkip = g_pLocalPlayer;
	src = g_pLocalPlayer->GetEyePosition();
	dst = src + forward;

	ray.Init(src, dst);

	g_pEngineTrace->TraceRay(ray, 0x46004003, &filter, &tr);

	if (!tr.m_pEnt)
		return;

	if (tr.m_pEnt->GetTeam() != 0 && tr.m_pEnt->GetTeam() != g_pLocalPlayer->GetTeam())
		g_pUserCmd->buttons &= IN_ATTACK;
}

inline void CRCS::Tick() {
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
}

inline void CGlow::GlowEntity(int index, Color clr) {
	static uintptr_t dwClient = (uintptr_t)GetModuleHandleA("client.dll");
	GlowStruct_t* GlowObjectArray = *(GlowStruct_t**)(dwClient + g_dwGlowObject);
	if (GlowObjectArray) {
		GlowStruct_t* GlowObject = &GlowObjectArray[index];

		if (GlowObject) {
			GlowObject->m_vGlowColor = clr.ToVector();
			GlowObject->m_flGlowAlpha = clr.GetAlphaFloat();
			GlowObject->m_bRenderWhenOccluded = true;
			GlowObject->m_bRenderWhenUnoccluded = false;
		}
	}
}

inline void CGlow::Tick() {
	for (int i = 0; i < 64; i++) {
		CBaseEntity* Entity = g_pClientEntityList->GetClientEntity(i);
		if (!Entity || Entity->GetDormant() || !Entity->GetHealth())
			return;

		Color clr = Entity->GetTeam() == g_pLocalPlayer->GetTeam() ? Color(255, 0, 255, 150) : Color(255, 255, 0, 255);
		GlowEntity(Entity->GetGlowIndex(), clr);
	}
}