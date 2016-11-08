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

class CAimbot {
public:
	void Tick();

private:
	CBaseEntity* GetTargetByFov();
	bool IsVisible(CBaseEntity* ent);

	float m_flSpeed = 5.f;
	float m_flFoV = 5.f;
	int m_iBone;
	float curAimTime, deltaTime;
};