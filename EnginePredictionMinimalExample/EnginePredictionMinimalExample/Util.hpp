#pragma once

#define INRANGE(x,a,b)    (x >= a && x <= b) 
#define getBits( x )    (INRANGE((x&(~0x20)),'A','F') ? ((x&(~0x20)) - 'A' + 0xa) : (INRANGE(x,'0','9') ? x - '0' : 0))
#define getByte( x )    (getBits(x[0]) << 4 | getBits(x[1]))

#define M_PI		3.14159265358979323846f
#define M_RADPI		57.295779513082f
#define M_PI_F		((float)(M_PI))	// Shouldn't collide with anything.
#define RAD2DEG( x  )  ( (float)(x) * (float)(180.f / M_PI_F) )
#define DEG2RAD( x  )  ( (float)(x) * (float)(M_PI_F / 180.f) )

namespace Util
{
	inline uintptr_t FindPattern(const char* moduleName, const char* pattern) {
		const char* pat = pattern;
		uintptr_t firstMatch = 0;
		uintptr_t rangeStart = (uintptr_t)GetModuleHandleA(moduleName);
		MODULEINFO miModInfo; GetModuleInformation(GetCurrentProcess(), (HMODULE)rangeStart, &miModInfo, sizeof(MODULEINFO));
		uintptr_t rangeEnd = rangeStart + miModInfo.SizeOfImage;
		for (uintptr_t pCur = rangeStart; pCur < rangeEnd; pCur++) {
			if (!*pat)
				return firstMatch;

			if (*(PBYTE)pat == '\?' || *(BYTE*)pCur == getByte(pat)) {
				if (!firstMatch)
					firstMatch = pCur;

				if (!pat[2])
					return firstMatch;

				if (*(PWORD)pat == '\?\?' || *(PBYTE)pat != '\?')
					pat += 3;

				else
					pat += 2;

			}
			else {
				pat = pattern;
				firstMatch = 0;
			}
		}
		return NULL;
	}

	template <typename Interface>
	inline Interface CreateInterface(const char *dll, const char *name) {
		return (Interface)(((void *(*)(const char *, void *))GetProcAddress(GetModuleHandleA(dll), "CreateInterface"))(name, 0));
	}

}

namespace Math
{
	inline void SinCos(float radians, float *sine, float *cosine) {
		*sine = sin(radians);
		*cosine = cos(radians);
	}

	inline void AngleVectors(QAngle q, Vector& vec) {
		float sp, sy, cp, cy;

		SinCos(DEG2RAD(q[1]), &sy, &cy);
		SinCos(DEG2RAD(q[0]), &sp, &cp);

		vec.x = cp*cy;
		vec.y = cp*sy;
		vec.z = -sp;
	}
}