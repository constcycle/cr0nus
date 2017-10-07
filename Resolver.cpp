#include "sdk.h"
#include "Resolver.h"
#include "global.h"
#include "BacktrackingHelper.h"
#include "Math.h"
#include "GameUtils.h"
#include "Menu.h"
CResolver* g_Resolver = new CResolver;

#define TICK_INTERVAL			( g_pGlobals->interval_per_tick )
#define TIME_TO_TICKS( dt )		( (int)( 0.5f + (float)(dt) / TICK_INTERVAL ) )
#define TICKS_TO_TIME( t )		( TICK_INTERVAL *( t ) )

CResolver::CTickRecord::CTickRecord(CBaseEntity* ent, int tickcount)
{
	m_flLowerBodyYawTarget = ent->LowerBodyYaw();
	m_angEyeAngles = ent->GetEyeAngles();
	m_flCycle = ent->GetCycle();
	m_flSimulationTime = ent->GetSimulationTime();
	m_nSequence = ent->GetSequence();
	m_vecOrigin = ent->GetOrigin();
	m_vecVelocity = ent->GetVelocity();
	//	m_flPoseParameter = ent->GetPoseParameters();
	m_angAbsAngles = ent->GetAbsAngles();
	m_vecAbsOrigin = ent->GetOrigin();
	this->tickcount = tickcount;
}
CResolver::CTickRecord::CTickRecord(CBaseEntity* ent, Vector EyeAngles, float LowerBodyYaw)
{
	m_flLowerBodyYawTarget = LowerBodyYaw;
	m_angEyeAngles = EyeAngles;//ent->GetEyeAngles();
	m_flCycle = ent->GetCycle();
	m_flSimulationTime = ent->GetSimulationTime();
	m_nSequence = ent->GetSequence();
	m_vecOrigin = ent->GetOrigin();
	m_vecVelocity = ent->GetVelocity();
	//	m_flPoseParameter = ent->GetPoseParameters();
	m_angAbsAngles = ent->GetAbsAngles();
	m_vecAbsOrigin = ent->GetOrigin();
	tickcount = G::UserCmdForBacktracking->tick_count;
}
float tolerance = 10.f;
const inline float GetDelta(float a, float b) {
	return abs(Math::NormalizeYaw(a - b));
}

const inline float LBYDelta(const CResolver::CTickRecord& v) {
	return v.m_angEyeAngles.y - v.m_flLowerBodyYawTarget;
}

const inline bool IsDifferent(float a, float b, float tolerance = 10.f) {
	return (GetDelta(a, b) > tolerance);
}

void CResolver::StoreVars(CBaseEntity* ent)
{
	if (m_arrInfos.at(ent->Index()).m_sRecords.size() >= 32)///static_cast<size_t>(Config->GetValue<int>("Resolver", "Ticks")))
		m_arrInfos.at(ent->Index()).m_sRecords.pop_back();

	m_arrInfos.at(ent->Index()).m_sRecords.push_front(CTickRecord(ent, G::UserCmdForBacktracking->tick_count));
}
void CResolver::StoreVars(CBaseEntity* ent, Vector EyeAngles, float LowerBodyYaw)
{
	if (m_arrInfos.at(ent->Index()).m_sRecords.size() >= 32)///static_cast<size_t>(Config->GetValue<int>("Resolver", "Ticks")))
		m_arrInfos.at(ent->Index()).m_sRecords.pop_back();

	m_arrInfos.at(ent->Index()).m_sRecords.push_front(CTickRecord(ent, EyeAngles, LowerBodyYaw));
}
bool HasStaticRealAngle(const std::deque<CResolver::CTickRecord>& l, float tolerance) {
	auto minmax = std::minmax_element(std::begin(l), std::end(l), [](const CResolver::CTickRecord& t1, const CResolver::CTickRecord& t2) { return t1.m_flLowerBodyYawTarget < t2.m_flLowerBodyYawTarget; });
	return (fabs(minmax.first->m_flLowerBodyYawTarget - minmax.second->m_flLowerBodyYawTarget) <= tolerance);
}

bool CResolver::HasStaticRealAngle_Wrapper(int index, float tolerance) 
{
	return HasStaticRealAngle(m_arrInfos[index].m_sRecords, tolerance);
}

bool HasStaticYawDifference(const std::deque<CResolver::CTickRecord>& l, float tolerance) {
	for (auto i = l.begin(); i < l.end() - 1;) 
	{
		if (GetDelta(LBYDelta(*i), LBYDelta(*++i)) > tolerance)
			return false;
	}
	return true;
}

bool HasSteadyDifference(const std::deque<CResolver::CTickRecord>& l, float tolerance, CBaseEntity* pEnt) {
	size_t misses = 0;
	for (size_t i = 0; i < l.size() - 1; i++) {
		float tickdif = static_cast<float>(l.at(i).m_flSimulationTime - l.at(i + 1).tickcount);
		float lbydif = GetDelta(l.at(i).m_flLowerBodyYawTarget, l.at(i + 1).m_flLowerBodyYawTarget);
		float ntickdif = static_cast<float>(G::UserCmdForBacktracking->tick_count - l.at(i).tickcount);
		if (((lbydif / tickdif) * ntickdif) > tolerance) 
			misses++;
	}
	return (misses <= (l.size() / 3));
}
int GetDifferentDeltas(const std::deque<CResolver::CTickRecord>& l, float tolerance) {
	std::vector<float> vec;
	for (auto var : l) {
		float curdelta = LBYDelta(var);
		bool add = true;
		for (auto fl : vec) {
			if (!IsDifferent(curdelta, fl, tolerance))
				add = false;
		}
		if (add)
			vec.push_back(curdelta);
	}
	return vec.size();
}

int GetDifferentLBYs(const std::deque<CResolver::CTickRecord>& l, float tolerance) {
	std::vector<float> vec;
	for (auto var : l) 
	{
		float curyaw = var.m_flLowerBodyYawTarget;
		bool add = true;
		for (auto fl : vec) 
		{
			if (!IsDifferent(curyaw, fl, tolerance))
				add = false;
		}
		if (add)
			vec.push_back(curyaw);
	}
	return vec.size();
}
float GetLBYByComparingTicks(const std::deque<CResolver::CTickRecord>& l, CBaseEntity* pEnt)
{
	int modulo = 1;//;Config->GetValue<int>("Resolver", "Modulo");
	int difangles = GetDifferentLBYs(l, tolerance);
	int inc = modulo * difangles;
	for (auto var : l) 
	{
		for (int lasttick = var.tickcount; lasttick < G::UserCmdForBacktracking->tick_count; lasttick += inc)
		{
			if (lasttick == G::UserCmdForBacktracking->tick_count)
				return var.m_flLowerBodyYawTarget;
		}
	}
	return 0.f;
}

float GetDeltaByComparingTicks(const std::deque<CResolver::CTickRecord>& l, CBaseEntity* pEnt) {
	int modulo = 1;// Config->GetValue<int>("Resolver", "Modulo");
	int difangles = GetDifferentDeltas(l, tolerance);
	int inc = modulo * difangles;
	for (auto var : l) 
	{
		for (int lasttick = var.tickcount; lasttick < G::UserCmdForBacktracking->tick_count; lasttick += inc) {
			if (lasttick == G::UserCmdForBacktracking->tick_count)
				return LBYDelta(var);
		}
	}
	return 0.f;
}
bool DeltaKeepsChanging(const std::deque<CResolver::CTickRecord>& cur, float tolerance) {
	return (GetDifferentDeltas(cur, tolerance) >(int) cur.size() / 2);
}

bool LBYKeepsChanging(const std::deque<CResolver::CTickRecord>& cur, float tolerance) {
	return (GetDifferentLBYs(cur, tolerance) > (int)cur.size() / 2);
}

float CResolver::ResolvePitch(CBaseEntity* pPlayer, float org_pitch)
{
	
	
	auto new_pitch = org_pitch;
	/*bruteforce method*/
	
		if (org_pitch > 181 && org_pitch < 271)
		{
			new_pitch = 269.0;
		}
		else if (org_pitch > 89 && org_pitch < 181)
		{
			new_pitch = 89.0;
		}
		else
		{
			if (!(pPlayer->GetFlags() & FL_ONGROUND))
			{
			
				new_pitch = 89;
			}
			else
			{

					/*proper dynamic fix*/
					if (org_pitch > 181 && org_pitch < 271)
					{
						new_pitch = 269.0;
					}
					else if (org_pitch > 89 && org_pitch < 181)
					{
						new_pitch = 89.0;
					}
					else if (org_pitch <= -66)
						new_pitch = 89;
					else if (org_pitch == 0)
						new_pitch = 89;

				//}
		}
	}
	

	return new_pitch;
}
enum at_target_antiaims
{
	Undefined,
	Backwards,
	Left, 
	Right
};

float CResolver::ResolveYaw(CBaseEntity* pPlayer, float org_yaw)
{
	auto index = pPlayer->Index();

	auto GetYawToNearestPlayer = [](CBaseEntity* pCenter) -> QAngle
	{
		auto get3ddist = [](Vector myCoords, Vector enemyCoords) -> float
		{

			return sqrt(
				pow(double(enemyCoords.x - myCoords.x), 2.0) +
				pow(double(enemyCoords.y - myCoords.y), 2.0) +
				pow(double(enemyCoords.z - myCoords.z), 2.0));

		};
		float best_dist = 9999;
		CBaseEntity* selected_player = nullptr;
		Vector vEyePos = pCenter->GetEyePosition();
		for (int i = 1; i < g_pGlobals->maxClients; i++)
		{

			CBaseEntity* pCBaseEntity = g_pEntitylist->GetClientEntity(i);
			if (pCBaseEntity)
			{
				if (pCBaseEntity->GetHealth() > 0 && !pCBaseEntity->IsDormant() && pCBaseEntity->GetTeamNum() != pCenter->GetTeamNum() && pCBaseEntity != pCenter)
				{
					float dist = get3ddist(vEyePos, pCBaseEntity->GetEyePosition());
					if (best_dist > dist)
					{
						best_dist = dist;
						selected_player = pCBaseEntity;
					}
				}
			}
		}
		if (selected_player)
		{
			return GameUtils::CalculateAngle(vEyePos, selected_player->GetEyePosition());
		}

		return QAngle(-1, -1, -1);
	};

	QAngle at_target = GetYawToNearestPlayer(pPlayer);
	at_target.y = Math::NormalizeYaw(at_target.y);

	auto new_yaw = org_yaw;

	bool is_moving = pPlayer->GetVelocity().Length() > 29.f;
	
	bool is_onground = pPlayer->GetFlags() & FL_ONGROUND;

	if (pPlayer->LowerBodyYaw() != LatestLowerBodyYaw[index] || (is_moving && is_onground))
	{
		LatestLowerBodyYaw[index] = pPlayer->LowerBodyYaw();

		OldYawDifference[index] = YawDifference[index];

		YawDifference[index] = Math::NormalizeYaw(new_yaw - pPlayer->LowerBodyYaw());

		aa_info[index] = "lby update";

		LatestLowerBodyYawUpdateTime[index] = g_pGlobals->curtime;//pPlayer->GetSimulationTime();

		return Math::NormalizeYaw(pPlayer->LowerBodyYaw());
	}

	if (is_moving)
	{
        aa_info[ index ] = "lby update";
	}
	else /*On ground resolver*/
	{
	
		if (((g_pGlobals->curtime - LatestLowerBodyYawUpdateTime[index]) >= 0.5))
		{
			new_yaw -= 180;
			aa_info[index] = "static";
		}
		else
		{
			auto cur = this->m_arrInfos.at(index).m_sRecords;

			if (cur.size() > 1)
			{
				if (HasStaticRealAngle(cur, 5))
					new_yaw = (cur.front().m_flLowerBodyYawTarget) + (Math::RandomFloat(0.f, 1.f) > 0.5f ? 10 : -10);
				else if (HasStaticYawDifference(cur, 10))
					new_yaw -=  (cur.front().m_angEyeAngles.y - cur.front().m_flLowerBodyYawTarget);
				else if (HasSteadyDifference(cur, 10, pPlayer)) {
					float tickdif = static_cast<float>(cur.front().tickcount - cur.at(1).tickcount);
					float lbydif = GetDelta(cur.front().m_flLowerBodyYawTarget, cur.at(1).m_flLowerBodyYawTarget);
					float ntickdif = static_cast<float>(TIME_TO_TICKS(g_pGlobals->curtime) - cur.front().tickcount);
					new_yaw = (lbydif / tickdif) * ntickdif;
				}
				else if (DeltaKeepsChanging(cur, 10))
					new_yaw -= GetDeltaByComparingTicks(cur, pPlayer);
				else if (LBYKeepsChanging(cur, 10))
					new_yaw = GetLBYByComparingTicks(cur, pPlayer);

				aa_info[index] = "stored";
			}
			else
			{
				// do all other logic here
				new_yaw = pPlayer->LowerBodyYaw() + Math::RandomFloat(-45, 45);
				aa_info[index] = "bruteforce";
			}

		}
	}

	new_yaw = Math::NormalizeYaw(new_yaw);

	return new_yaw;
}



void CResolver::Run(CBaseEntity* pPlayer)
{
	Vector* eye_angles = pPlayer->GetEyeAnglesPtr();

	static Vector old_resolved[64];

	int index = pPlayer->GetIndex();

	int CBaseEntityServerID = GetServerSideID(pPlayer->Index());
	if (PlayerData[CBaseEntityServerID].MissedShots >= 2)
	{
		PlayerData[CBaseEntityServerID].LastAnglesHit.Invalidate();
	}


	Vector eye_ang = pPlayer->GetEyeAngles();

	if (RagebotElements.Ragebot_Corrections_Resolver->Checked)
	{
		

		float pitch = ResolvePitch(pPlayer, g_BacktrackHelper->PlayerRecord[index].EyeAngles.x/*eye_ang.x*/);
		float yaw = ResolveYaw(pPlayer, g_BacktrackHelper->PlayerRecord[index].EyeAngles.y/*eye_ang.y*/);

		eye_angles->x = pitch;


		eye_angles->y = yaw;

		old_resolved[index] = Vector(pitch, yaw);
	}
	
}