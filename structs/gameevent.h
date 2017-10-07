#pragma once
class IGameEvent
{
public:
	virtual ~IGameEvent() = 0;
	virtual const char *GetName() const = 0;

	virtual bool  IsReliable() const = 0;
	virtual bool  IsLocal() const = 0;
	virtual bool  IsEmpty(const char *keyName = NULL) = 0;

	virtual bool  GetBool(const char *keyName = NULL, bool defaultValue = false) = 0;
	virtual int   GetInt(const char *keyName = NULL, int defaultValue = 0) = 0;
	virtual unsigned long GetUint64(const char *keyName = NULL, unsigned long defaultValue = 0) = 0;
	virtual float GetFloat(const char *keyName = NULL, float defaultValue = 0.0f) = 0;
	virtual const char *GetString(const char *keyName = NULL, const char *defaultValue = "") = 0;
	virtual const wchar_t *GetWString(const char *keyName, const wchar_t *defaultValue = L"") = 0;

	virtual void SetBool(const char *keyName, bool value) = 0;
	virtual void SetInt(const char *keyName, int value) = 0;
	virtual void SetUint64(const char *keyName, unsigned long value) = 0;
	virtual void SetFloat(const char *keyName, float value) = 0;
	virtual void SetString(const char *keyName, const char *value) = 0;
	virtual void SetWString(const char *keyName, const wchar_t *value) = 0;
};
extern int GetServerSideID(int CBaseEntityIndex);
class IGameEventListener2
{
public:
	virtual	~IGameEventListener2(void) {};

	// FireEvent is called by EventManager if event just occured
	// KeyValue memory will be freed by manager if not needed anymore
	virtual void FireGameEvent(IGameEvent *event) = 0;
	virtual int  GetEventDebugID(void) = 0;
};

class IGameEventManager
{
public:
	bool AddListener(IGameEventListener2 *listener, const char *name, bool bServerSide)
	{
		typedef bool(__thiscall* OriginalFn)(PVOID, IGameEventListener2*, const char*, bool);
		return getvfunc<OriginalFn>(this, 3)(this, listener, name, bServerSide);
	}

	bool FireEventClientSide(IGameEvent *event)
	{
		typedef bool(__thiscall* OriginalFn)(PVOID, IGameEvent*);
		return  getvfunc<OriginalFn>(this, 9)(this, event);
	}
};

class cGameEvent : public IGameEventListener2
{


public:

	void FireGameEvent(IGameEvent *event);
	int GetEventDebugID();
	void RegisterSelf();
	void Register();


};

extern cGameEvent g_Event;

struct ResolveData
{
	int ShotsFiredAtTarget = 0;
	QAngle LastAnglesHit;
	QAngle AdjustedAngles = QAngle(0, 0, 0);
	QAngle LastAttemptedAngles = QAngle(0, 0, 0);
	int MissedShots = 0;
};
extern std::map<int, ResolveData> PlayerData;