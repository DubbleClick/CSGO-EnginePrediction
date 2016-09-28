#pragma once

using CreateMoveFn = bool(__stdcall*)(float, CUserCmd*);
extern CreateMoveFn oCreateMove;

namespace Hooks
{
	extern bool __stdcall CreateMove(float, CUserCmd*);
}