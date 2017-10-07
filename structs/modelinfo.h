struct mstudiobbox_t
{
	int					bone;
	int					group;				// intersection group
	Vector				bbmin;				// bounding box
	Vector				bbmax;
	int					szhitboxnameindex;	// offset to the name of the hitbox.
	int					unused[3];
	float				radius;
	int					unused2[4];

	const char* pszHitboxName()
	{
		if (szhitboxnameindex == 0)
			return "";

		return ((char*)this) + szhitboxnameindex;
	}

	mstudiobbox_t() {}

private:
	mstudiobbox_t(const mstudiobbox_t& vOther);
};

struct mstudiobone_t
{
	int					sznameindex;
	inline char * const pszName(void) const { return ((char *)this) + sznameindex; }
	int		 			parent;		// parent bone
	int					bonecontroller[6];	// bone controller index, -1 == none

	// default values
	Vector				pos;
	Quaternion			quat;
	Vector			rot;
	// compression scale
	Vector				posscale;
	Vector				rotscale;

	VMatrix			poseToBone;
	Quaternion			qAlignment;
	int					flags;
	int					proctype;
	int					procindex;		// procedural rule
	mutable int			physicsbone;	// index into physically simulated bone
	inline void *pProcedure() const { if (procindex == 0) return NULL; else return  (void *)(((byte *)this) + procindex); };
	int					surfacepropidx;	// index into string tablefor property name
	inline char * const pszSurfaceProp(void) const { return ((char *)this) + surfacepropidx; }
	int					contents;		// See BSPFlags.h for the contents flags

	int					unused[8];		// remove as appropriate

	mstudiobone_t(){}
private:
	// No copy constructors allowed
	mstudiobone_t(const mstudiobone_t& vOther);
};

struct mstudiohitboxset_t
{
	int					sznameindex;
	inline char * const	pszName(void) const { return ((char *)this) + sznameindex; }
	int					numhitboxes;
	int					hitboxindex;
	inline mstudiobbox_t *pHitbox(int i) const { return (mstudiobbox_t *)(((byte *)this) + hitboxindex) + i; };
};

struct studiohdr_t
{
	int					id;
	int					version;
	long				checksum;		// this has to be the same in the phy and vtx files to load!
	inline const char *	pszName(void) const { return name; }
	char				name[64];
	int					length;

	Vector				eyeposition;	// ideal eye position
	Vector				illumposition;	// illumination center
	Vector				hull_min;		// ideal movement hull size
	Vector				hull_max;
	Vector				view_bbmin;		// clipping bounding box
	Vector				view_bbmax;

	int					flags;
	int					numbones;			// bones
	int					boneindex;
	inline mstudiobone_t *pBone(int i) const { return (mstudiobone_t *)(((byte *)this) + boneindex) + i; };
	int					RemapSeqBone(int iSequence, int iLocalBone) const;	// maps local sequence bone to global bone
	int					RemapAnimBone(int iAnim, int iLocalBone) const;		// maps local animations bone to global bone

	int					numbonecontrollers;		// bone controllers
	int					bonecontrollerindex;
	inline void *pBonecontroller(int i) const { return  (((byte *)this) + bonecontrollerindex) + i; };

	int					numhitboxsets;
	int					hitboxsetindex;

	// Look up hitbox set by index
	mstudiohitboxset_t	*pHitboxSet(int i) const
	{
		return (mstudiohitboxset_t *)(((byte *)this) + hitboxsetindex) + i;
	};

	// Calls through to hitbox to determine size of specified set
	inline mstudiobbox_t *pHitbox(int i, int set) const
	{
		mstudiohitboxset_t const *s = pHitboxSet(set);
		if (!s)
			return NULL;

		return s->pHitbox(i);
	};

	// Calls through to set to get hitbox count for set
	inline int			iHitboxCount(int set) const
	{
		mstudiohitboxset_t const *s = pHitboxSet(set);
		if (!s)
			return 0;

		return s->numhitboxes;
	};
	int					numlocalanim;			// animations/poses
	int					localanimindex;		// animation descriptions
	inline /*mstudioanimdesc_t*/void **pLocalAnimdesc(int i) const { if (i < 0 || i >= numlocalanim) i = 0; return (/*mstudioanimdesc_t*/ void**)(((byte *)this) + localanimindex) + i; };

	int					numlocalseq;				// sequences
	int					localseqindex;
	inline /*mstudioseqdesc_t*/void* *pLocalSeqdesc(int i) const { if (i < 0 || i >= numlocalseq) i = 0; return (/*mstudioseqdesc_t*/ void**)(((byte *)this) + localseqindex) + i; };

	//public:
	bool				SequencesAvailable() const
	{
		//if (numincludemodels == 0)
			return true;
		//return (GetVirtualModel() != NULL);
	}

	int					GetNumSeq() const;
	/*mstudioanimdesc_t*/void*	&pAnimdesc(int i) const;
	/*mstudioseqdesc_t*/ void*	&pSeqdesc(int i) const;
	int					iRelativeAnim(int baseseq, int relanim) const;	// maps seq local anim reference to global anim index
	int					iRelativeSeq(int baseseq, int relseq) const;		// maps seq local seq reference to global seq index

	//private:
	mutable int			activitylistversion;	// initialization flag - have the sequences been indexed?
	mutable int			eventsindexed;
	//public:
	int					GetSequenceActivity(int iSequence);
	void				SetSequenceActivity(int iSequence, int iActivity);
	int					GetActivityListVersion(void);
	void				SetActivityListVersion(int version) const;
	int					GetEventListVersion(void);
	void				SetEventListVersion(int version);

	// raw textures
	int					numtextures;

	////////////////////////////MORE THINGS 


	studiohdr_t() {}

private:
	// No copy constructors allowed
	studiohdr_t(const studiohdr_t& vOther);

	friend struct virtualmodel_t;
};
struct model_t;
//0x3F00FB33
class CModelInfo
{
public:
	studiohdr_t* GetStudioModel(const model_t* Model)
	{

			typedef studiohdr_t*(__thiscall* Fn)(void*, const model_t*);
			return getvfunc<Fn>(this, 30)(this, Model);

	}
	void GetModelRenderBounds(const model_t *model, Vector& mins, Vector& maxs)
	{
		typedef void(__thiscall* Fn)(void*, const model_t*, Vector&, Vector&);
		return getvfunc<Fn>(this, 7)(this, model, mins, maxs);
	}
	int GetModelIndex(const char* ModelName)
	{
		typedef int(__thiscall* OriginalFn)(PVOID, const char*);
		return getvfunc<OriginalFn>(this, 2)(this, ModelName);
	}
	const char* GetModelName(const model_t *model)
	{
		typedef const char* (__thiscall* OriginalFn)(PVOID, const model_t*);
		return getvfunc<OriginalFn>(this, 3)(this, model);
	}
	void GetModelMaterials(const model_t *model, int count, IMaterial** ppMaterial)
	{
		typedef void(__thiscall* OriginalFn)(PVOID, const model_t*, int, IMaterial**);
		getvfunc<OriginalFn>(this, 17)(this, model, count, ppMaterial);
	}/*
	void SetMaterialVarFlag(MaterialVarFlags_t flag, bool on)
	{
		typedef void(__thiscall* OriginalFn)(PVOID, MaterialVarFlags_t, bool);
		getvfunc<OriginalFn>(this, 29)(this, flag, on);
	}
	void SetColorModulation(float const* blend)
	{
		typedef void(__thiscall* OriginalFn)(PVOID, float const*);
		return getvfunc<OriginalFn>(this, 6)(this, blend);
	}*/
};