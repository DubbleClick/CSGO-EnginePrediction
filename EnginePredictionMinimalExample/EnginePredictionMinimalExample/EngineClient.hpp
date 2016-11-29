#pragma once

class IEngineClient {
public:
	virtual void VirtualFunction0() = 0;
	virtual void VirtualFunction1() = 0;
	virtual void VirtualFunction2() = 0;
	virtual void VirtualFunction3() = 0;
	virtual void VirtualFunction4() = 0;
	virtual void VirtualFunction5() = 0;
	virtual void VirtualFunction6() = 0;
	virtual void VirtualFunction7() = 0;
	virtual void VirtualFunction8() = 0;
	virtual void VirtualFunction9() = 0;
	virtual void VirtualFunction10() = 0;
	virtual void VirtualFunction11() = 0;
	virtual int  GetLocalPlayer() = 0;

	void SetViewAngles(QAngle& angles) {
		typedef void(__thiscall* T)(void*, QAngle&);
		(*reinterpret_cast<T**>(this))[19](this, angles);
	}
};

extern IEngineClient* g_pEngineClient;