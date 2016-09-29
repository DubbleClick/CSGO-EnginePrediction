#pragma once

class CBaseEntity;

class VectorAligned : public Vector {
public:
	VectorAligned() {}

	VectorAligned(const Vector &vec) {
		this->x = vec.x;
		this->y = vec.y;
		this->z = vec.z;
	}
	float w;
};

struct Ray_t {
	Ray_t() {}

	VectorAligned		m_Start;
	VectorAligned		m_Delta;
	VectorAligned		m_StartOffset;
	VectorAligned		m_Extents;
	const void*			m_pWorldAxisTransform;
	bool				m_IsRay;
	bool				m_IsSwept;

	void Init(Vector vecStart, Vector vecEnd) {
		m_Delta = VectorAligned(vecEnd - vecStart);
		m_IsSwept = (m_Delta.LengthSqr() != 0);
		m_Extents.Zero();
		m_pWorldAxisTransform = NULL;
		m_IsRay = true;
		m_StartOffset.Zero();
		m_Start = vecStart;
	}

	void Init(Vector const& start, Vector const& end, Vector const& mins, Vector const& maxs) {
		m_Delta = end - start;

		m_pWorldAxisTransform = NULL;
		m_IsSwept = (m_Delta.LengthSqr() != 0);

		m_Extents = maxs - mins;
		m_Extents *= 0.5f;
		m_IsRay = (m_Extents.LengthSqr() < 1e-6);

		// Offset m_Start to be in the center of the box...
		m_StartOffset = maxs + mins;
		m_StartOffset *= 0.5f;
		m_Start = start + m_StartOffset;
		m_StartOffset *= -1.0f;
	}
};

enum TraceType_t {
	TRACE_EVERYTHING = 0,
	TRACE_WORLD_ONLY,				// NOTE: This does *not* test static props!!!
	TRACE_ENTITIES_ONLY,			// NOTE: This version will *not* test static props
	TRACE_EVERYTHING_FILTER_PROPS,	// NOTE: This version will pass the IHandleEntity for props through the filter, unlike all other filters
};

class ITraceFilter {
public:
	virtual bool ShouldHitEntity(CBaseEntity* pEntityHandle, int contentsMask) {
		return !(pEntityHandle == pSkip);
	}
	virtual TraceType_t	GetTraceType() {
		return TRACE_EVERYTHING;
	}
	void* pSkip;
};

struct cplane_t {
	Vector	normal;
	float	dist;
	byte	type;
	byte	signbits;
	byte	pad[2];
};

class CBaseTrace {
public:
	Vector			startpos;
	Vector			endpos;
	cplane_t		plane;

	float			fraction;

	int				contents;
	unsigned short	dispFlags;

	bool			allsolid;
	bool			startsolid;

	CBaseTrace() {}
};

struct csurface_t {
	const char*		name;
	short			surfaceProps;
	unsigned short	flags;
};

class trace_t : public CBaseTrace {
public:
	float                   fractionleftsolid;
	csurface_t              surface;
	int                     hitgroup;
	short                   physicsbone;
	unsigned short          worldSurfaceIndex;
	CBaseEntity*            m_pEnt;
	int                     hitbox;

	trace_t() {}

private:
	trace_t(const trace_t& vOther);
};

class IEngineTrace {
	virtual void VirtualFunction0() = 0;
	virtual void VirtualFunction1() = 0;
	virtual void VirtualFunction2() = 0;
	virtual void VirtualFunction3() = 0;
	virtual void VirtualFunction4() = 0;
public:
	virtual void	TraceRay(const Ray_t &ray, unsigned int fMask, ITraceFilter *pTraceFilter, trace_t *pTrace) = 0;
};

extern IEngineTrace* g_pEngineTrace;