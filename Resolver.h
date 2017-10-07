#pragma once
#include <deque>
class CResolver
{
public:
	struct CTickRecord;

	struct CValidTick {
		explicit operator CTickRecord() const;

		explicit operator bool() const noexcept {
			return m_flSimulationTime > 0.f;
		}

		float m_flPitch = 0.f;
		float m_flYaw = 0.f;
		float m_flSimulationTime = 0.f;
		CBaseEntity* m_pCBaseEntity = nullptr;
	};

	struct CTickRecord {
		CTickRecord() {}
		CTickRecord(CBaseEntity* ent, int tickcount);
		CTickRecord(CBaseEntity* ent, Vector EyeAngles, float LowerBodyYaw);

		explicit operator bool() const noexcept {
			return m_flSimulationTime > 0.f;
		}

		bool operator>(const CTickRecord& others) {
			return (m_flSimulationTime > others.m_flSimulationTime);
		}
		bool operator>=(const CTickRecord& others) {
			return (m_flSimulationTime >= others.m_flSimulationTime);
		}
		bool operator<(const CTickRecord& others) {
			return (m_flSimulationTime < others.m_flSimulationTime);
		}
		bool operator<=(const CTickRecord& others) {
			return (m_flSimulationTime <= others.m_flSimulationTime);
		}
		bool operator==(const CTickRecord& others) {
			return (m_flSimulationTime == others.m_flSimulationTime);
		}

		float m_flLowerBodyYawTarget = 0.f;
		QAngle m_angEyeAngles = QAngle(0, 0, 0);
		float m_flCycle = 0.f;
		float m_flSimulationTime = 0.f;
		int m_nSequence = 0;
		Vector m_vecOrigin = Vector(0, 0, 0);
		Vector m_vecAbsOrigin = Vector(0, 0, 0);
		Vector m_vecVelocity = Vector(0, 0, 0);
		std::array<float, 24> m_flPoseParameter = {};
		QAngle m_angAbsAngles = QAngle(0, 0, 0);
		CValidTick validtick;
		int tickcount = 0;
	};

	class CResolveInfo
	{
	public:
		std::deque<CTickRecord> m_sRecords;
	protected:

		bool	m_bEnemyShot; //priority
		bool	m_bLowerBodyYawChanged;
		bool	m_bBacktrackThisTick;
	};

	enum resolver_stages
	{
		lby_delta,
		at_bw,
		at_left,
		at_half_right,
		at_half_left, 
		at_right
	};

public:
	void StoreVars(CBaseEntity* ent);
	void StoreVars(CBaseEntity* ent, Vector EyeAngles, float LowerBodyYaw);
	bool HasStaticRealAngle_Wrapper(int index, float tolerance);
	float ResolvePitch(CBaseEntity* pPlayer, float org_pitch);
	float ResolveYaw(CBaseEntity* pPlayer, float org_yaw);
	void Run(CBaseEntity* pPlayer);


	std::string aa_info[64];

	

	float resolve_infos[64];

private:
	float LatestLowerBodyYaw[64];
	bool LbyUpdated[64];
	float YawDifference[64];
	float OldYawDifference[64];
	float LatestLowerBodyYawUpdateTime[64];

	std::array<CResolveInfo, 32> m_arrInfos;




}; extern CResolver* g_Resolver;


