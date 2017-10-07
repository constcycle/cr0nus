
#include <math.h>
enum TraceType_t
{
	TRACE_EVERYTHING = 0,
	TRACE_WORLD_ONLY,
	TRACE_ENTITIES_ONLY,
	TRACE_EVERYTHING_FILTER_PROPS,
};
struct cplane_t
{
	Vector  normal;
	float   dist;
	BYTE    type;
	BYTE    signbits;
	BYTE    pad[2];
};
struct csurface_t
{
	const char*             name;
	short                   surfaceProps;
	unsigned short  flags;
};


class CBaseTrace
{
public:
	Vector                  startpos;
	Vector                 endpos;
	cplane_t                plane;
	float                   fraction;
	int                             contents;
	unsigned short  dispFlags;
	bool                    allsolid;
	bool                    startsolid;
};



class CGameTrace : public CBaseTrace
{
public:


	float			fractionleftsolid;	// time we left a solid, only valid if we started in solid
	csurface_t		surface;			// surface hit (impact surface)

	int				hitgroup;			// 0 == generic, non-zero is specific body part

	short			physicsbone;		// physics bone hit by trace in studio
	unsigned short	worldSurfaceIndex;	// Index of the msurface2_t, if applicable

	CBaseEntity*  m_pEnt;
	int			hitbox;

	bool DidHitWorld() const;

	
	bool DidHitNonWorldCBaseEntity() const;

	bool DidHit() const;
	

};




typedef CGameTrace trace_t;
typedef float vec_t;
class  VectorAligned : public Vector
{
public:
	inline VectorAligned(void) {};
	inline VectorAligned(vec_t X, vec_t Y, vec_t Z)
	{
		//		Init(X,Y,Z);
		//		X = 0.0f; Y=0.0f ;Z =0.0f;
	}

#ifdef VECTOR_NO_SLOW_OPERATIONS

private:
	// No copy constructors allowed if we're in optimal mode
	VectorAligned(const VectorAligned& vOther);
	VectorAligned(const Vector &vOther);

#else
public:
	explicit VectorAligned(const Vector &vOther)
	{
		//		Init(vOther.X, vOther.Y, vOther.Z);
		//		vOther.X=0.0f;vOther.Y=0.0f;vOther.Z =0.0;
	}

	VectorAligned& operator=(const Vector &vOther)
	{
		//		Init(vOther.X, vOther.Y, vOther.Z);
		x = vOther.x; y = vOther.y; z = vOther.z;
		return *this;
	}

#endif
	float w;	// this space is used anyway
};
struct Ray_t
{
	VectorAligned        m_Start;
	VectorAligned        m_Delta;
	VectorAligned        m_StartOffset;
	VectorAligned        m_Extents;

	const   matrix3x4* m_pWorldAxisTransform;

	bool    m_IsRay;
	bool    m_IsSwept;

	Ray_t() : m_pWorldAxisTransform(NULL) { }

	void Init(Vector& vecStart, Vector& vecEnd)
	{
		m_Delta = vecEnd - vecStart;

		m_IsSwept = (m_Delta.LengthSqr() != 0);

		m_Extents = Vector(0, 0, 0);

		m_pWorldAxisTransform = NULL;

		m_IsRay = true;

		m_StartOffset = Vector(0, 0, 0);

		m_Start = vecStart;
	}

	void Init(Vector const& start, Vector const& end, Vector const& mins, Vector const& maxs)
	{
		Assert(&end);
		VectorSubtract(end, start, m_Delta);

		m_pWorldAxisTransform = NULL;
		m_IsSwept = (m_Delta.LengthSqr() != 0);

		VectorSubtract(maxs, mins, m_Extents);
		m_Extents *= 0.5f;
		m_IsRay = (m_Extents.LengthSqr() < 1e-6);

		// Offset m_Start to be in the center of the box...
		VectorAdd(mins, maxs, m_StartOffset);
		m_StartOffset *= 0.5f;
		VectorAdd(start, m_StartOffset, m_Start);
		m_StartOffset *= -1.0f;
	}
};
class ITraceFilter
{
public:
	virtual bool ShouldHitCBaseEntity(CBaseEntity *pCBaseEntity, int contentsMask) = 0;
	virtual TraceType_t    GetTraceType() const = 0;
};

class CTraceFilterWorldAndPropsOnly : public ITraceFilter
{
public:
	bool ShouldHitCBaseEntity(CBaseEntity* pCBaseEntityHandle, int contentsMask)
	{
		return false;
	}
	virtual TraceType_t GetTraceType() const
	{
		return TRACE_EVERYTHING;
	}
};

class CTraceWorldOnly : public ITraceFilter
{
public:
	bool ShouldHitCBaseEntity(CBaseEntity* pCBaseEntityHandle, int contentsMask)
	{
		return false;
	}

	TraceType_t GetTraceType() const
	{
		return TRACE_WORLD_ONLY;
	}
};

class CTraceFilter : public ITraceFilter
{
public:
	bool ShouldHitCBaseEntity(CBaseEntity* pCBaseEntityHandle, int contentsMask)
	{
		return !(pCBaseEntityHandle == pSkip);
	}

	virtual TraceType_t GetTraceType() const
	{
		return TRACE_EVERYTHING;
	}

	void* pSkip;
};
class CTraceCBaseEntity : public ITraceFilter
{
public:
	bool ShouldHitCBaseEntity(CBaseEntity* pCBaseEntityHandle, int contentsMask)
	{
		return (pCBaseEntityHandle == pHit);
	}
	virtual TraceType_t	GetTraceType() const
	{
		return TRACE_ENTITIES_ONLY;
	}
	void* pHit;
};
class CTraceFilterNoPlayer : public CTraceFilter
{
public:
	CTraceFilterNoPlayer() {}
	virtual bool ShouldHitCBaseEntity(CBaseEntity *pServerCBaseEntity, int contentsMask)
	{
		if (pServerCBaseEntity)
			return !pServerCBaseEntity->IsPlayer();
		return false;
	}
};
class CEngineTrace
{
public:

	void TraceRay(const Ray_t &ray, unsigned int fMask, CTraceFilter *pTraceFilter, trace_t *pTrace)
	{
		typedef void(__thiscall* oTraceRay)(PVOID, const Ray_t&, unsigned int, CTraceFilter *, trace_t*);
		getvfunc<oTraceRay>(this, 5)(this, ray, fMask, pTraceFilter, pTrace);
	}
	void TraceRay_NEW(const Ray_t &ray, unsigned int fMask, ITraceFilter* pTraceFilter, trace_t* pTrace)
	{
		typedef void(__thiscall* oTraceRay)(void*, const Ray_t&, unsigned int, ITraceFilter*, trace_t*);
		getvfunc<oTraceRay>(this, 5)(this, ray, fMask, pTraceFilter, pTrace);
	}
	void ClipRayToCBaseEntity(const Ray_t& ray, unsigned int nMask, CBaseEntity* pCBaseEntity, trace_t* pTrace)
	{
		typedef void(__thiscall* oClipRayToTrace)(PVOID, const Ray_t&, unsigned int, CBaseEntity*, trace_t*);
		getvfunc<oClipRayToTrace>(this, 3)(this, ray, nMask, pCBaseEntity, pTrace);
	}
	void CBaseEntityTraceRay(const Ray_t &ray, unsigned int fMask, CTraceCBaseEntity *pTraceFilter, trace_t *pTrace)
	{
		typedef void(__thiscall* oTraceRay)(PVOID, const Ray_t&, unsigned int, CTraceCBaseEntity*, trace_t*);
		getvfunc<oTraceRay>(this, 5)(this, ray, fMask, pTraceFilter, pTrace);
	}
	int	GetPointContents(const Vector &vecAbsPosition, int contentsMask = MASK_ALL, CBaseEntity* ppCBaseEntity = NULL)
	{
		typedef int(__thiscall* fnGetPointContents)(void*, const Vector&, int, CBaseEntity*);
		return getvfunc<fnGetPointContents>(this, 0)(this, vecAbsPosition, contentsMask, ppCBaseEntity);
	}
	void BestPointX(CBaseEntity *targetPlayer, Vector &final, bool negative, float pointscale)
	{
		trace_t tr;
		Ray_t ray;
		CTraceCBaseEntity* filter = new CTraceCBaseEntity();

		float Hit = 10 / 100;
		Hit *= pointscale;

		if (negative)
			Hit *= -1;

		filter->pHit = targetPlayer;
		ray.Init(final + Vector(Hit, 0, 0), final);
		CBaseEntityTraceRay(ray, MASK_SHOT, filter, &tr);

		final = tr.endpos;
		
	}
	void BestPointY(CBaseEntity *targetPlayer, Vector &final, bool negative, float pointscale)
	{
		trace_t tr;
		Ray_t ray;
		CTraceCBaseEntity* filter = new CTraceCBaseEntity();

		float Hit = 10 / 100;
		Hit *= pointscale;

		if (negative)
			Hit *= -1;

		filter->pHit = targetPlayer;
		ray.Init(final + Vector(0, Hit, 0), final);
		CBaseEntityTraceRay(ray, MASK_SHOT, filter, &tr);

		final = tr.endpos;
	}
	void BestPointZ(CBaseEntity *targetPlayer, Vector &final, bool negative, float pointscale)
	{
		trace_t tr;
		Ray_t ray;
		CTraceCBaseEntity* filter = new CTraceCBaseEntity();

		float Hit = 10 / 100;
		Hit *= pointscale;

		if (negative)
			Hit *= -1;

		filter->pHit = targetPlayer;
		ray.Init(final + Vector(0, 0, Hit), final);
		CBaseEntityTraceRay(ray, MASK_SHOT, filter, &tr);

		final = tr.endpos;
	}
	bool IsVisible(CBaseEntity* pLocalClientBaseCBaseEntity, Vector vecOrigin, Vector vecFinal, CBaseEntity* pClientBaseCBaseEntity)
	{
		Ray_t ray;
		ray.Init(vecOrigin, vecFinal);

		CTraceFilter TraceFilter;
		TraceFilter.pSkip = pLocalClientBaseCBaseEntity;

		trace_t trace;
		TraceRay(ray, MASK_SHOT, &TraceFilter, &trace);
	
		return (trace.m_pEnt == pClientBaseCBaseEntity || trace.fraction >= 1.0f);
	}
	bool IsVisible(CBaseEntity* pLocalClientBaseCBaseEntity, Vector vecOrigin, Vector vecFinal, CBaseEntity* pClientBaseCBaseEntity, int& hitgroup)
	{
		Ray_t ray;
		ray.Init(vecOrigin, vecFinal);

		CTraceFilter* TraceFilter = new CTraceFilter();
		TraceFilter->pSkip = pLocalClientBaseCBaseEntity;

		trace_t trace;
		TraceRay(ray, MASK_SHOT, TraceFilter, &trace);
		hitgroup = trace.hitgroup;
		delete TraceFilter;
		return (trace.m_pEnt == pClientBaseCBaseEntity || trace.fraction >= 1.0f);
	}

};

