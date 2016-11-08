#include "sdk.hpp"

void CBunnyHop::Tick() {
	if (g_pUserCmd->buttons & IN_JUMP) {
		if (!(g_pLocalPlayer->GetFlags() & FL_ONGROUND))
			g_pUserCmd->buttons &= ~IN_JUMP;
	}
}

void CTrigger::Tick() {
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

void CRCS::Tick() {
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

void CGlow::GlowEntity(int index, Color clr) {
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

void CGlow::Tick() {
	for (int i = 0; i < 64; i++) {
		CBaseEntity* Entity = g_pClientEntityList->GetClientEntity(i);
		if (!Entity || Entity->GetDormant() || !Entity->GetHealth())
			continue;

		Color clr = Entity->GetTeam() == g_pLocalPlayer->GetTeam() ? Color(255, 0, 255, 150) : Color(255, 255, 0, 255);
		GlowEntity(Entity->GetGlowIndex(), clr);
	}
}

bool CAimbot::IsVisible(CBaseEntity* ent) {
	Ray_t ray;
	trace_t tr;
	ray.Init(g_pLocalPlayer->GetEyePosition(), ent->GetBonePosition(m_iBone));

	ITraceFilter filter;
	filter.pSkip = g_pLocalPlayer;

	g_pEngineTrace->TraceRay(ray, 24705U, &filter, &tr); //MASK_VISIBLE

	if (tr.m_pEnt == ent) {
		return true;
	}

	return false;

}

CBaseEntity* CAimbot::GetTargetByFov() {
	float bestfov = m_flFoV;
	CBaseEntity* target = nullptr;
	QAngle viewangles = g_pUserCmd->viewangles + g_pLocalPlayer->GetPunchAngles()*2.f;

	for (int i = 0; i < 64; i++) {
		CBaseEntity* Entity = g_pClientEntityList->GetClientEntity(i);
		if (!Entity || Entity->GetDormant() || !Entity->GetHealth() || !IsVisible(Entity))
			continue;

		float fov = Math::GetFov(g_pUserCmd->viewangles, Math::CalcAngle(g_pLocalPlayer->GetEyePosition(), Entity->GetBonePosition(m_iBone)));
		if (fov < bestfov) {
			bestfov = fov;
			target = Entity;
		}
	}
	return target;
}

void CAimbot::Tick() {
	static float oldServerTime = g_pLocalPlayer->GetTickBase() * g_pGlobals->interval_per_tick;
	float serverTime = g_pLocalPlayer->GetTickBase() * g_pGlobals->interval_per_tick;
	deltaTime = serverTime - oldServerTime;
	oldServerTime = serverTime;

	CBaseEntity* target = GetTargetByFov();

	QAngle dst = Math::CalcAngle(g_pLocalPlayer->GetEyePosition(), target->GetBonePosition(m_iBone));
	QAngle src = g_pUserCmd->viewangles;

	dst -= g_pLocalPlayer->GetPunchAngles() * 2.f;

	QAngle delta = dst - src;

	delta.Clamp();

	if (!(delta.x == 0.f && delta.y == 0.f && delta.z == 0.f)) {
		float finalTime = delta.Length() / m_flSpeed;

		curAimTime += deltaTime;

		if (curAimTime > finalTime)
			curAimTime = finalTime;

		float percent = curAimTime / finalTime;

		curAimTime = 0.f;

		delta *= percent;
		dst = src + delta;
	}

	g_pUserCmd->viewangles = dst.Clamp();
}