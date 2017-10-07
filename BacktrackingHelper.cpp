#include "sdk.h"
#include "BacktrackingHelper.h"
#include "global.h"
#include "xor.h"
#include "Math.h"
template<class T, class U>
inline T clamp(T in, U low, U high)
{
	if (in <= low)
		return low;
	else if (in >= high)
		return high;
	else
		return in;
}
#define TICK_INTERVAL			( g_pGlobals->interval_per_tick )
#define TIME_TO_TICKS( dt )		( floorf(( 0.5f + (float)(dt) / TICK_INTERVAL ) ) )
#define TICKS_TO_TIME( t )		( TICK_INTERVAL *( t ) )
CBacktrackHelper* g_BacktrackHelper = new CBacktrackHelper;
int CBacktrackHelper::GetDesiredTickCount(float flTargetTime)
{
	static ConVar* cl_interp_ratio = g_pCvar->FindVar(XorStr("cl_interp_ratio"));
	static ConVar* cl_updaterate = g_pCvar->FindVar(XorStr("cl_updaterate"));
	static ConVar* sv_client_min_interp_ratio = g_pCvar->FindVar(XorStr("sv_client_min_interp_ratio"));
	static ConVar* sv_client_max_interp_ratio = g_pCvar->FindVar(XorStr("sv_client_max_interp_ratio"));
	static ConVar* sv_minupdaterate = g_pCvar->FindVar(XorStr("sv_minupdaterate"));
	static ConVar* sv_maxupdaterate = g_pCvar->FindVar(XorStr("sv_maxupdaterate"));
	static ConVar* cl_interp = g_pCvar->FindVar(XorStr("cl_interp"));
	static ConVar* cl_interpolate = g_pCvar->FindVar(XorStr("cl_interpolate"));


	

	//UpdateRate
	float interp = cl_interp->GetValue();
	float flUpdateRate = cl_updaterate->GetValueN();
	float  interp_ratio = cl_interp_ratio->GetValueN();

	if (sv_client_min_interp_ratio->GetValue() >  interp_ratio)
		interp_ratio = sv_client_min_interp_ratio->GetValue();
	if (interp_ratio > sv_client_max_interp_ratio->GetValue())
		interp_ratio = sv_client_max_interp_ratio->GetValue();
	if (sv_maxupdaterate->GetValueN() <= flUpdateRate)
		flUpdateRate = sv_maxupdaterate->GetValueN();
	if (sv_minupdaterate->GetValueN() > flUpdateRate)
		flUpdateRate = sv_minupdaterate->GetValueN();

	float v4 = interp_ratio / flUpdateRate;
	if (v4 > interp)
		interp = v4;


	//I really hope this was a macro which is compiler inserted, otherwise shame on aw
	return (((1.0 / g_pGlobals->interval_per_tick) * flTargetTime) + 0.5) //TIME_TO_TICKS SHIT
		+ (((1.0 / g_pGlobals->interval_per_tick) * interp) + 0.5);
}
int CBacktrackHelper::GetLatencyTicks()
{
	double v0; // st7@0
	INetChannelInfo* v1; // esi@1
	INetChannelInfo* v2; // eax@1
	float v3; // ST08_4@1
	float v4; // ST0C_4@1

	v1 = (INetChannelInfo*)g_pEngine->GetNetChannelInfo();
	v2 = (INetChannelInfo*)g_pEngine->GetNetChannelInfo();

	v3 = v1->GetAvgLatency(Typetype_t::TYPE_LOCALPLAYER);
	v4 = v2->GetAvgLatency(Typetype_t::TYPE_GENERIC);

	float interval_per_tick = 1.0f / g_pGlobals->interval_per_tick;

	return floorf(((v3 + v4) * interval_per_tick) + 0.5f);
}
int CBacktrackHelper::GetEstimateServerTickCount()
{
	double v0; // st7@0
	INetChannelInfo* v1; // esi@1
	INetChannelInfo* v2; // eax@1
	float v3; // ST08_4@1
	float v4; // ST0C_4@1

	v1 = (INetChannelInfo*)g_pEngine->GetNetChannelInfo();
	v2 = (INetChannelInfo*)g_pEngine->GetNetChannelInfo();

	v3 = v1->GetAvgLatency(Typetype_t::TYPE_LOCALPLAYER);
	v4 = v2->GetAvgLatency(Typetype_t::TYPE_GENERIC);

	return floorf(((v3 + v4) / g_pGlobals->interval_per_tick) + 0.5f) + 1 + G::UserCmdForBacktracking->tick_count;
}
float CBacktrackHelper::GetEstimateServerTime()
{
	double v0; // st7@0
	INetChannelInfo* v1; // esi@1
	INetChannelInfo* v2; // eax@1
	float v3; // ST08_4@1
	float v4; // ST0C_4@1

	v1 = (INetChannelInfo*)g_pEngine->GetNetChannelInfo();
	v2 = (INetChannelInfo*)g_pEngine->GetNetChannelInfo();

	v3 = v1->GetAvgLatency(Typetype_t::TYPE_LOCALPLAYER);
	v4 = v2->GetAvgLatency(Typetype_t::TYPE_GENERIC);

	//return floorf(((v3 + v4) / g_pGlobals->interval_per_tick) + 0.5f) + 1 + G::UserCmdForBacktracking->tick_count;*/

		return v3 + v4 +  TICKS_TO_TIME(1) + TICKS_TO_TIME(G::UserCmdForBacktracking->tick_count);

}
float CBacktrackHelper::GetNetworkLatency()
{
	// Get true latency
	INetChannelInfo *nci = g_pEngine->GetNetChannelInfo();
	if (nci)
	{
		//float IncomingLatency = nci->GetAvgLatency(FLOW_INCOMING); //ppl say use only this one, but meh
		float OutgoingLatency = nci->GetLatency(0);
		return OutgoingLatency;
	}
	return 0.0f;
}
ConVar* minupdate;
ConVar* maxupdate;
ConVar * updaterate;
ConVar * interprate;
ConVar* cmin;
ConVar* cmax;
 ConVar* interp;
float CBacktrackHelper::GetLerpTime()
{
	if(!minupdate)
		minupdate =g_pCvar->FindVar(XorStr("sv_minupdaterate"));
	if(!maxupdate)
		maxupdate = g_pCvar->FindVar(XorStr("sv_maxupdaterate"));
	if(!updaterate)
		updaterate = g_pCvar->FindVar(XorStr("cl_updaterate"));
	if(!interprate)
		interprate = g_pCvar->FindVar(XorStr("cl_interp_ratio"));
	if(!cmin)
		cmin = g_pCvar->FindVar(XorStr("sv_client_min_interp_ratio"));
	if(!cmax)
		cmax = g_pCvar->FindVar(XorStr("sv_client_max_interp_ratio"));
	if(!interp)
		interp = g_pCvar->FindVar(XorStr("cl_interp"));

	float UpdateRate = updaterate->GetValue();
	float LerpRatio = interprate->GetValue();

	return max(LerpRatio / UpdateRate, interp->GetValue());
}

void CBacktrackHelper::UpdateBacktrackRecords(CBaseEntity* pPlayer) 
{
	int i = pPlayer->Index();

	

	for (int j = g_BacktrackHelper->PlayerRecord[i].records.size() - 1; j >= 0; j--)
	{
		float lerptime = g_BacktrackHelper->GetLerpTime();
		float desired_time = g_BacktrackHelper->PlayerRecord[i].records.at(j).m_flSimulationTime + lerptime;
		float estimated_time = g_BacktrackHelper->GetEstimateServerTime();


		float SV_MAXUNLAG = 1.0f;


		float latency = g_BacktrackHelper->GetNetworkLatency();
		float m_flLerpTime = g_BacktrackHelper->GetLerpTime();
		float correct = clamp<float>(latency + m_flLerpTime, 0.0f, SV_MAXUNLAG);

		float deltaTime = correct - (estimated_time + lerptime - desired_time);

		if (fabs(deltaTime) > 0.2f)
			g_BacktrackHelper->PlayerRecord[i].records.erase(g_BacktrackHelper->PlayerRecord[i].records.begin() + j);
	}

	
	static Vector old_origin[64];

	

	if (PlayerRecord[i].records.size() > 0 && pPlayer->GetSimulationTime() == PlayerRecord[i].records.back().m_flSimulationTime) //already got such a record
		return;

	if (PlayerRecord[i].records.size() > 0 && PlayerRecord[i].records.back().m_flSimulationTime > pPlayer->GetSimulationTime())//Invalid lag record, maybe from diffrent game?
	{
		PlayerRecord[i].records.clear();
		return;
	}

	Vector cur_origin = pPlayer->GetOrigin();

	Vector v = cur_origin - old_origin[i];

	bool breaks_lagcomp = v.LengthSqr() > 4096.f;

	old_origin[i] = cur_origin;

	//if (breaks_lagcomp)
	//{
		//add a bool to tick_record called extrapolate and set it to true here, then don't return but let the aimbot extrapolate, for now we just skip that
		//return;
	//}


	

	tick_record new_record;

	new_record.needs_extrapolation = breaks_lagcomp;

	static float OldLower[64];
	

	PlayerRecord[i].LowerBodyYawTarget = pPlayer->LowerBodyYaw();

	

	
	new_record.m_angEyeAngles = pPlayer->GetEyeAngles();
	new_record.m_flCycle = pPlayer->GetCycle();
	new_record.m_flSimulationTime = pPlayer->GetSimulationTime();
	new_record.m_flAnimTime = pPlayer->GetAnimationTime();
	new_record.bLowerBodyYawUpdated = false;
	new_record.m_nSequence = pPlayer->GetSequence();
	new_record.m_vecOrigin = pPlayer->GetOrigin();
	new_record.m_vecVelocity = pPlayer->GetVelocity();
	new_record.m_flUpdateTime = g_pGlobals->curtime;
	new_record.backtrack_time = new_record.m_flSimulationTime + GetLerpTime();

	if (PlayerRecord[i].LowerBodyYawTarget != OldLower[i] || (pPlayer->GetFlags() & FL_ONGROUND && pPlayer->GetVelocity().Length() > 29.f))
		new_record.bLowerBodyYawUpdated = true;

	for (int i = 0; i < 24; i++)
		new_record.m_flPoseParameter[i] = *(float*)((DWORD)pPlayer + offys.m_flPoseParameter + sizeof(float) * i);

	int sequence = pPlayer->GetSequence();
	if (sequence == Activity::ACT_PLAYER_IDLE_FIRE || sequence == Activity::ACT_PLAYER_RUN_FIRE || sequence == Activity::ACT_PLAYER_WALK_FIRE || sequence == Activity::ACT_PLAYER_CROUCH_FIRE || sequence == Activity::ACT_PLAYER_CROUCH_WALK_FIRE)
	{
		new_record.shot_in_that_record = true;
	}

	pPlayer->SetupBones(new_record.boneMatrix, 128, 0x00000100, g_pGlobals->curtime);

	

	OldLower[i] = PlayerRecord[i].LowerBodyYawTarget;

	PlayerRecord[i].records.push_back(new_record);

}
void CBacktrackHelper::UpdateExtrapolationRecords(CBaseEntity* pPlayer)
{
	int index = pPlayer->Index();
	if (pPlayer->GetSimulationTime() == this->SimRecord[index][0].simulation_time)
		return;

	for (int i = 7; i > 0; i--)
	{
		this->SimRecord[index][i].acceleration = this->SimRecord[index][i-1].acceleration;
		this->SimRecord[index][i].origin = this->SimRecord[index][i - 1].origin;
		this->SimRecord[index][i].simulation_time = this->SimRecord[index][i - 1].simulation_time;
		this->SimRecord[index][i].update_time = this->SimRecord[index][i - 1].update_time;
		this->SimRecord[index][i].velocity = this->SimRecord[index][i - 1].velocity;
	}

	this->SimRecord[index][0].simulation_time = pPlayer->GetSimulationTime();
	this->SimRecord[index][0].update_time = g_pGlobals->curtime;
	this->SimRecord[index][0].origin = pPlayer->GetOrigin();

	int lost_ticks = TIME_TO_TICKS(this->SimRecord[index][0].simulation_time) - TIME_TO_TICKS(this->SimRecord[index][1].simulation_time);

	this->SimRecord[index][0].simulation_time_increasment_per_tick = (this->SimRecord[index][0].simulation_time - this->SimRecord[index][1].simulation_time) / lost_ticks;

	/*calculate velocity by ourselves*/
	Vector velocity = this->SimRecord[index][0].origin - this->SimRecord[index][1].origin;
	/*divide through lost ticks to get the velocity per tick*/
	velocity /= lost_ticks;

	this->SimRecord[index][0].velocity = pPlayer->GetVelocity();//velocity;
	
}
#include "PredictionSystem.h"
void CBacktrackHelper::ExtrapolatePosition(CBaseEntity* pPlayer, Vector& position, float &simtime, Vector velocity)
{
	auto AirAccelerate = [](CBaseEntity* pPlayer, Vector &wishdir, float wishspeed, float accel, Vector &velo)
	{
		float wishspd = wishspeed;

		if (wishspd > 30.f)
			wishspd = 30.f;

		float currentspeed = DotProduct(pPlayer->GetVelocity(), wishdir);

		float addspeed = wishspd - currentspeed;

		if (addspeed <= 0)
			return;

		float accelspeed = accel * wishspeed * g_pGlobals->frametime * pPlayer->GetFriction();

		if (accelspeed > addspeed)
			accelspeed = addspeed;

		velo += accelspeed * wishdir;
	};
	auto GetSomeStrangeSimTime = [](simulation_record pre_latest_record, simulation_record latest_record, float difference) -> float
	{
		float v2; // ST00_4@1
		float v3; // xmm2_4@1
		float v4; // ST00_4@1
		float v5; // xmm2_4@1
		float result; // xmm0_4@2

		v2 = atan2(latest_record.velocity.x, latest_record.velocity.y);
		v3 = v2 * 57.295776;
		v4 = atan2(pre_latest_record.velocity.x, pre_latest_record.velocity.y);
		v5 = v3 - (v4 * 57.295776);
		if (v5 <= 180.0)
		{
			if (v5 < -180.0)
				v5 = v5 + 360.0;
			result = v5 / (difference/*latest_record.simulation_time - pre_latest_record.simulation_time*/);
		}
		else
		{
			result = (v5 - 360.0) / (difference/*latest_record.simulation_time - pre_latest_record.simulation_time*/);
		}
		return result;
	};
	

	int index = pPlayer->GetIndex();

	simulation_record latest_record = this->SimRecord[index][0];
	simulation_record pre_latest_record = this->SimRecord[index][1];

	int latency_ticks = GetLatencyTicks();

	

	

	
	Vector vel = velocity;

	float s_time = simtime;

	INetChannelInfo* nci = g_pEngine->GetNetChannelInfo();

	float latency = nci->GetAvgLatency(0) + nci->GetAvgLatency(1);

	float ticks_per_second = 1.0f / g_pGlobals->interval_per_tick;

	float server_time = ((floorf(((latency)*ticks_per_second) + 0.5) + G::UserCmdForBacktracking->tick_count + 1) * g_pGlobals->interval_per_tick);
	
	float server_time_difference = server_time - s_time;
	if (server_time_difference > 1.0f)
		server_time_difference = 1.0f;

	float sim_time_difference = g_pGlobals->curtime - latest_record.update_time;
	//proper method
	int ticks_choked = /*TIME_TO_TICKS(latest_record.simulation_time - pre_latest_record.simulation_time_increasment_per_tick);*/TIME_TO_TICKS(sim_time_difference);



	if (ticks_choked <= 15)
	{
		latency_ticks = 1;
		if (ticks_choked < 1)
			ticks_choked = 1;
	}
	else
		ticks_choked = 15;

	int total_difference = floorf((server_time_difference * ticks_per_second) + 0.5) - ticks_choked;

	vel.z -= g_pCvar->FindVar("sv_gravity")->GetValue() * g_pGlobals->interval_per_tick;

	Ray_t ray;
	trace_t tr;
	CTraceFilterWorldAndPropsOnly filter;



	if (total_difference < 0)
	{
		return;
	}
	else
	{
		do
		{
			int chokes = ticks_choked;

			//test, aw has that do statement
			do
			{

				ray.Init(position, position + (vel* g_pGlobals->interval_per_tick), G::LocalPlayer->GetCollision()->VecMins(), G::LocalPlayer->GetCollision()->VecMaxs());

				g_pEngineTrace->TraceRay_NEW(ray, MASK_SOLID, &filter, &tr);

				if (tr.fraction > 0)
				{
					position += vel * (g_pGlobals->interval_per_tick);
					simtime += g_pGlobals->interval_per_tick;
				}
				else
				{

					return;
				}
				/*if (tr.DidHitWorld())
				{
					position = tr.endpos;
					if (position.z < tr.endpos.z)
						vel.z *= -1;
					else
					{

						chokes = 0;
						total_difference = 0;
						break;
					}
				}*/

				/*CUserCmd* cmd = G::UserCmdForBacktracking;
				Math::VectorAngles(pPlayer->GetVelocity(), cmd->viewangles);
				cmd->aimdirection = cmd->viewangles;
				cmd->buttons = 0;
				cmd->command_number = G::UserCmdForBacktracking->command_number;
				cmd->forwardmove = 400;
				cmd->sidemove = 0;
				cmd->hasbeenpredicted = false;
				cmd->impulse = G::UserCmdForBacktracking->impulse;
				cmd->mousedx = 0;
				cmd->mousedy = 0;
				cmd->random_seed = G::UserCmdForBacktracking->random_seed;
				cmd->tick_count = TIME_TO_TICKS(pPlayer->GetSimulationTime());
				cmd->upmove = 0;
				cmd->weaponselect = 0;
				cmd->weaponsubtype = 0;

				g_pPrediction->RunCommand(pPlayer, cmd, g_pMoveHelper);
				*/
				chokes--;
				/*vel.z -= (g_pCvar->FindVar("sv_gravity")->GetValue() * 0.5f * g_pGlobals->interval_per_tick);
				vel.z += (vel.z * g_pGlobals->interval_per_tick);

				float fmove = 400;
				float smove = 0;

				QAngle angle = QAngle();
				Math::VectorAngles(vel, angle);

				Vector forward = Vector();
				Vector right = Vector();
				Vector up = Vector();

				Math::AngleVectors(angle, &forward, &right, &up);

				forward.z = 0;
				right.z = 0;

				forward.NormalizeInPlace();
				right.NormalizeInPlace();

				Vector wishvel = forward * fmove + right * smove;

				wishvel.z = 0;

				Vector wishdir = wishvel;

				float wishspeed = wishdir.Length();

				if (wishspeed != 0 && (wishspeed > pPlayer->GetMaxSpeed()))
				{
				wishvel *= (pPlayer->GetMaxSpeed() / wishspeed);
				wishspeed = pPlayer->GetMaxSpeed();
				}

				AirAccelerate(pPlayer, wishdir, wishspeed, g_pCvar->FindVar("sv_airaccelerate")->GetValue(), vel);*/
			} while (chokes);

			total_difference -= ticks_choked;
		} while (total_difference >= 0);
	}
	//position.z = z_origin;
	//By notwav
	/*
float TimeTotal =  nci->GetAvgLatency(FLOW_OUTGOING) + nci->GetAvgLatency(FLOW_INCOMING) + EnemyPing + FudgeFactor;
 
for ( ; TotalTotal >= MinimumTime; TimeTotal -= nci->GetAvgLatency(FLOW_OUTGOING) )*/

	
	
	/*
	if (!G::UserCmdForBacktracking)
		return;

	CUserCmd* test = G::UserCmdForBacktracking;

	QAngle angle;
	SpreadVectorAngles(pPlayer->GetVelocity(), angle);

	test->upmove = 0;
	test->forwardmove = 400;
	test->sidemove = 0;
	test->aimdirection = angle;
	test->viewangles = angle;
	test->buttons = NULL;
	test->hasbeenpredicted = false;
	test->tick_count = TIME_TO_TICKS(pPlayer->GetSimulationTime());
	

	//g_pMoveHelper->SetHost(pPlayer);

	
	for (int i = ticks_choked; i >= 0; i--)
	{
		//ghetto way
		//position += vel;
		//simtime += sim_record.simulation_time_increasment_per_tick;
		//
		//g_pPrediction->RunCommand(pPlayer, test, g_pMoveHelper);
		CMoveData m_MoveData;
		memset(&m_MoveData, 0, sizeof(m_MoveData));
		g_pMoveHelper->SetHost(pPlayer);
		g_pPrediction->SetupMove(pPlayer, test, g_pMoveHelper, &m_MoveData);
		g_pGameMovement->ProcessMovement(pPlayer, &m_MoveData);
		g_pPrediction->FinishMove(pPlayer, test, &m_MoveData);
	}
	position = pPlayer->GetOrigin();
	simtime = pPlayer->GetSimulationTime();*/
	

	/*float gravity = (g_pCvar->FindVar("sv_gravity")->GetValue() * 0.5f * g_pGlobals->interval_per_tick);
	float basevelocity = (pPlayer->GetBaseVelocity().z * g_pGlobals->interval_per_tick);

	Vector vel = velocity;  vel.z -= gravity; vel.z += basevelocity;
	
	vel *= g_pGlobals->interval_per_tick;

	Vector old_origin, old_velocity,  acceleration;

	for (int i = ticks_choked; i > 0; i--)
	{
		old_origin = position;
		old_velocity = vel;

		//should be using that tbh
		//vel += acceleration;

		position += vel;

		//vel = position - old_origin;
	
		acceleration = vel - old_velocity;	
	}*/	
}

void CBacktrackHelper::LagCompensate(CBaseEntity* pPlayer)
{
	/*int index = pPlayer->Index();
	Vector org_delta = this->PlayerRecord[index].record[0].m_vecOrigin - this->PlayerRecord[index].record[1].m_vecOrigin;
	Vector min = pPlayer->GetCollision()->VecMins(), max = pPlayer->GetCollision()->VecMaxs();
	if (org_delta.Length() > 64)
	{
		for (int i = 0; i < 32; i++)
		{
			org_delta = this->PlayerRecord[index].record[i].m_vecOrigin - this->PlayerRecord[index].record[i + 1].m_vecOrigin;
			int LerpTicks = TIME_TO_TICKS(this->GetLerpTime());
			int desired = TIME_TO_TICKS(this->PlayerRecord[index].record[i].m_flSimulationTime) + LerpTicks;
			int estimated = this->GetEstimateServerTickCount();


			float SV_MAXUNLAG = 1.0f;


			float latency = g_BacktrackHelper->GetNetworkLatency();
			float m_flLerpTime = g_BacktrackHelper->GetLerpTime();
			float correct = clamp<float>(latency + m_flLerpTime, 0.0f, SV_MAXUNLAG);

			float deltaTime = correct - (TICKS_TO_TIME(estimated + LerpTicks - desired));

			if (org_delta.Length2D() <= 64 && fabs(deltaTime) <= 0.2f)
			{
				*(Vector*)((DWORD)pPlayer + offys.m_vecOrigin) = this->PlayerRecord[index].record[i].m_vecOrigin;
				*(float*)((DWORD)pPlayer + offys.m_flSimulationTime) = this->PlayerRecord[index].record[i].m_flSimulationTime;
				*(float*)((DWORD)pPlayer + offys.m_flCycle) = this->PlayerRecord[index].record[i].m_flCycle;
				*(int*)((DWORD)pPlayer + offys.m_nSequence) = this->PlayerRecord[index].record[i].m_nSequence;
				*(Vector*)((DWORD)pPlayer + 0x110) = this->PlayerRecord[index].record[i].m_vecVelocity;
			
				//g_pDebugOverlay->AddBoxOverlay(this->PlayerRecord[index].record[0].m_vecOrigin, min, max, pPlayer->GetEyeAngles(), 0, 0, 0, 255, g_pGlobals->interval_per_tick );
				//g_pDebugOverlay->AddBoxOverlay(this->PlayerRecord[index].record[i].m_vecOrigin, min, Vector(max.x, max.y, max.z + 52), Vector(0, pPlayer->GetEyeAngles().y, 0), 255, 255, 255, 55, g_pGlobals->interval_per_tick);
				break;
			}
		}
	}*/
}
/*
float CHistory::GetLerpTime()
{
	float flUpdateRate = g_pICvar->FindVar( "cl_updaterate" )->GetFloat();

	static float flMinUpdateRate = g_pICvar->FindVar( "sv_minupdaterate" )->GetFloat();
	static float flMaxUpdateRate = g_pICvar->FindVar( "sv_maxupdaterate" )->GetFloat();

	if( flMinUpdateRate && flMaxUpdateRate )
		flUpdateRate = clamp( flUpdateRate, flMaxUpdateRate, flMaxUpdateRate );

	float flLerpRatio = g_pICvar->FindVar( "cl_interp_ratio" )->GetFloat();

	if( flLerpRatio == 0 )
		flLerpRatio = 1.0f;

	float flLerpAmount = g_pICvar->FindVar( "cl_interp" )->GetFloat();

	static float flMin = g_pICvar->FindVar( "sv_client_min_interp_ratio" )->GetFloat();
	static float flMax = g_pICvar->FindVar( "sv_client_max_interp_ratio" )->GetFloat();

	if( flMin && flMax && flMin != -1 )
		flLerpRatio = clamp( flLerpRatio, flMin, flMax );
	else
		if( flLerpRatio == 0 )
			flLerpRatio = 1.0f;

	float flLerpTime = max( flLerpAmount, flLerpRatio / flUpdateRate );

	return flLerpTime;
}

int CHistory::TimeToTicks( float flSimulationTime )
{
	return ( int )( 0.5f + flSimulationTime / g_pGlobalVars->interval_per_tick );
}

float CHistory::TicksToTime( int iTicks )
{
	return g_pGlobalVars->interval_per_tick * iTicks;
}

void CHistory::InitLagRecord()
{
	for( int i = 0; i <= 32; i++ )
		for( int j = 0; j < 9; j++ )
			g_History.m_LagRecord[ i ][ j ].m_flSimulationTime = 0.0f;
}

int CHistory::GetServerTick()
{
	ValveSDK::INetChannelInfo* pNC = g_pEngine->GetNetChannelInfo();

	if( !pNC )
		return -1.0f;

	float flOut = pNC->GetLatency( FLOW_OUTGOING );
	float flInc = pNC->GetLatency( FLOW_INCOMING );
	return floorf( ( ( flOut + flInc ) / g_pGlobalVars->interval_per_tick ) + 0.5f ) + 1 + m_iStartingTickCount;
}

void CHistory::RecordCBaseEntity( CBaseEntity* pCBaseEntity )
{
	int nIndex = pCBaseEntity->GetIndex();
	CLagRecord* m_LagRecords = this->m_LagRecord[ nIndex ];

	if( !pCBaseEntity || !pCBaseEntity->GetGetHealth() > 0 )
	{
		for( int i = 0; i < 9; i++ )
			m_LagRecords[ i ].m_flSimulationTime = 0.0f;
	}

	if( pCBaseEntity->IsDormant() )	//Do not store useless data...
		return;

	float flSimTime = pCBaseEntity->GetSimulationTime();// +this->GetLerpTime();

	int nHigestRecordIndex = -1;
	float flHigestSimTime = 0.0f;
	for( int i = 0; i < 9; i++ )
	{
		if( m_LagRecords[ i ].m_flSimulationTime > flSimTime )	//Invalid lag record, maybe from diffrent game?
			m_LagRecords[ i ].m_flSimulationTime = 0.0f;

		if( m_LagRecords[ i ].m_flSimulationTime == 0.0f )
			continue;

		if( m_LagRecords[ i ].m_flSimulationTime == flSimTime )
			return;	//Already contains log of current simulation time.

		if( m_LagRecords[ i ].m_flSimulationTime > flHigestSimTime )
		{
			nHigestRecordIndex = i;
			flHigestSimTime = m_LagRecords[ i ].m_flSimulationTime;
		}
	}

	nHigestRecordIndex++;
	nHigestRecordIndex = nHigestRecordIndex % 9;

	m_LagRecords[ nHigestRecordIndex ].m_vecOrigin = pCBaseEntity->GetOrigin();
	Vector vVel;
	pCBaseEntity->GetEstimatedVelocity( vVel );
	m_LagRecords[ nHigestRecordIndex ].m_vecVelocity = vVel;
	m_LagRecords[ nHigestRecordIndex ].m_flSimulationTime = pCBaseEntity->GetSimulationTime();
	m_LagRecords[ nHigestRecordIndex ].m_vecAngles = *( Vector* )( ( DWORD )pCBaseEntity + Offsets::Player::m_angEyeAngles );
	m_LagRecords[ nHigestRecordIndex ].m_flCycle = *( float* )( ( DWORD )pCBaseEntity + Offsets::Player::m_flCycle );
	m_LagRecords[ nHigestRecordIndex ].m_nSequence = *( int* )( ( DWORD )pCBaseEntity + Offsets::Player::m_nSequence );
	m_LagRecords[ nHigestRecordIndex ].m_nFlags = *( int* )( ( DWORD )pCBaseEntity + Offsets::Player::m_fFlags );
	m_LagRecords[ nHigestRecordIndex ].m_flLowerBodyYawTarget = *( float* )( ( DWORD )pCBaseEntity + Offsets::Player::m_flLowerBodyYawTarget );

	for( int i = 0; i < 24; i++ )
		m_LagRecords[ nHigestRecordIndex ].m_flPoseParameter[ i ] = *( float* )( ( DWORD )pCBaseEntity + Offsets::Player::m_flPoseParameter + sizeof( float ) * i );
}

void CHistory::ApplyLagRecord( CBaseEntity* pCBaseEntity, CLagRecord _LagRecord )
{
	if( !pCBaseEntity || _LagRecord.m_flSimulationTime == 0.0f )
		return;

	*( Vector* )( ( DWORD )pCBaseEntity + Offsets::Player::m_vecOrigin ) = _LagRecord.m_vecOrigin;
	*( Vector* )( ( DWORD )pCBaseEntity + Offsets::Player::m_vecVelocity ) = _LagRecord.m_vecVelocity;
	*( float* )( ( DWORD )pCBaseEntity + Offsets::Player::m_flSimulationTime ) = _LagRecord.m_flSimulationTime;
	*( Vector* )( ( DWORD )pCBaseEntity + Offsets::Player::m_angEyeAngles ) = _LagRecord.m_vecAngles;
	*( float* )( ( DWORD )pCBaseEntity + Offsets::Player::m_flCycle ) = _LagRecord.m_flCycle;
	*( int* )( ( DWORD )pCBaseEntity + Offsets::Player::m_nSequence ) = _LagRecord.m_nSequence;
	*( int* )( ( DWORD )pCBaseEntity + Offsets::Player::m_fFlags ) = _LagRecord.m_nFlags;
	*( float* )( ( DWORD )pCBaseEntity + Offsets::Player::m_flLowerBodyYawTarget ) = _LagRecord.m_flLowerBodyYawTarget;

	for( int i = 0; i < 24; i++ )
		*( float* )( ( DWORD )pCBaseEntity + Offsets::Player::m_flPoseParameter + sizeof( float ) * i ) = _LagRecord.m_flPoseParameter[ i ];

	if( pCBaseEntity )
	{
		*( DWORD* )( ( DWORD )pCBaseEntity + Offsets::Player::m_nForceBone + 0x20 ) = 0;				// m_BoneAccessor.SetWritableBones( 0 )

		static DWORD dwIBC = Base::Util::PatternSearch(  "client.dll", ( PBYTE )"\x80\x3D\x00\x00\x00\x00\x00\x74\x16\xA1", "xx????xxxx", NULL, NULL );
		reinterpret_cast<void( __fastcall* )( void* )>( dwIBC )( pCBaseEntity );
	}
}

int CHistory::FakeLagFix( CBaseEntity* pCBaseEntity, int nHistoryIndex, bool bStrict )
{
	int nIndex = pCBaseEntity->GetIndex();
	CLagRecord* m_LagRecords = this->m_LagRecord[ nIndex ];

	CLagRecord recentLR = m_LagRecords[ nHistoryIndex ];
	CLagRecord prevLR;
	if( nHistoryIndex == 0 )
		prevLR = m_LagRecords[ 8 ];
	else
		prevLR = m_LagRecords[ nHistoryIndex - 1 ];

	if( recentLR.m_flSimulationTime == 0.0f )
		return -1;

	int iServerTick = this->GetServerTick();
	int iLerpTicks = this->TimeToTicks( GetLerpTime() );
	float flCorrect = this->TicksToTime( iLerpTicks );
	int iTargetTickCount = this->TimeToTicks( recentLR.m_flSimulationTime ) + iLerpTicks;
	int iMaxFutureServerTick = iServerTick;

	ValveSDK::ConVar* sv_max_usercmd_future_ticks = g_pICvar->FindVar( "sv_max_usercmd_future_ticks" );

	if( sv_max_usercmd_future_ticks )
		iMaxFutureServerTick += sv_max_usercmd_future_ticks->GetInt();

	ValveSDK::INetChannelInfo* pNCNfo = g_pEngine->GetNetChannelInfo();

	if( pNCNfo )
	{
		float flLatOut = pNCNfo->GetLatency( FLOW_OUTGOING );

		flCorrect += flLatOut;
	}

	flCorrect = clamp( flCorrect, 0.0f, 1.0f );	//sv_maxunlag

	float flDeltaTime = flCorrect - TicksToTime( iServerTick - iTargetTickCount );

	if( fabsf( flDeltaTime ) > 0.2f ) //Too big deltatime, can't lagfix here
	{
		return -1;
	}

	if( iTargetTickCount > iMaxFutureServerTick ) //Cannot go that much forward
	{
		return -1;
	}

	if( prevLR.m_flSimulationTime == 0.0f )
	{	//Found only one valid record, let's use this simulation time if it's valid.
		if( bStrict )
		{
			return -1;
		}
		else
		{
			ApplyLagRecord( pCBaseEntity, recentLR );

			return iTargetTickCount + 1;
		}
	}

	if( ( recentLR.m_vecOrigin - prevLR.m_vecOrigin ).LengthSqr() > 4096.f ) //Lost track record
	{
		m_iShouldExtrapolateHistory = nHistoryIndex;
		return -1;
	}

	ApplyLagRecord( pCBaseEntity, recentLR );

	return iTargetTickCount + 1;
	}*/



/*void LagCompensation::logCBaseEntity(CBaseEntity *player)
{
	int idx = player->getIdx();
	LagRecord *m_LagRecords = this->m_LagRecord[idx];
 
	if (!player || !player->getLife() > 0)
	{
		for (int i = 0; i < 9; i++) // reset sim time on dead records
		{
			m_LagRecords[i].m_fSimulationTime = 0.0f;
		}
	}
 
	float simTime = player->getSimulTime();
 
	int highestRecordIdx = -1;
	float highestSimTime = 0.0f;
 
	for (int i = 0; i < 9; i++) // here we do some checks so we don't have invalid records and optimize things so we don't store an
	{							// entity which already contains the current sim time
		if (m_LagRecords[i].m_fSimulationTime > simTime)
			m_LagRecords[i].m_fSimulationTime = 0.0f;
 
		if (m_LagRecords[i].m_fSimulationTime == 0.0f)
			continue;
 
		if (m_LagRecords[i].m_fSimulationTime == simTime)
			return;
 
		if (m_LagRecords[i].m_fSimulationTime > highestSimTime)
		{
			highestRecordIdx = i;
			highestSimTime = m_LagRecords[i].m_fSimulationTime;
		}
	}
 
	highestRecordIdx++;
	highestRecordIdx = highestRecordIdx % 9;
 
	m_LagRecords[highestRecordIdx].m_bIsFixed = false;
	m_LagRecords[highestRecordIdx].m_iTargetID = idx;
 
	m_LagRecords[highestRecordIdx].m_vAbsOrigin = player->getAbsOrigin();
	m_LagRecords[highestRecordIdx].m_vecVelocity = player->getVel();
	m_LagRecords[highestRecordIdx].m_fSimulationTime = player->getSimulTime();
	m_LagRecords[highestRecordIdx].m_vecAngles = player->getEyeAngles();
	m_LagRecords[highestRecordIdx].m_flCycle = player->getCycle();
	m_LagRecords[highestRecordIdx].m_nSequence = player->getSequence();
	m_LagRecords[highestRecordIdx].flags = player->getFlags();
	m_LagRecords[highestRecordIdx].m_flLowerBodyYawTarget = player->getLowBodYtarg();
	m_LagRecords[highestRecordIdx].m_vecMins = player->getCollideable()->vecMins();
	m_LagRecords[highestRecordIdx].m_vecMax = player->getCollideable()->vecMax();
	m_LagRecords[highestRecordIdx].m_fFallVel = player->getFallVel();
 
	// commented out since giving weird results, the backtracked entity is crouching, also this is bad anim fix, currently doing without anim fix
	/*for (int i = 0; i < 24; i++)
		m_LagRecords[highestRecordIdx].m_flPoseParameter[i] = player->getPoseParams(i);*/
//}
/*
void LagCompensation::setCBaseEntity(CBaseEntity *player, LagRecord record)
{
	if (!player || record.m_fSimulationTime == 0.0f)
	{
		record.m_bIsFixed = false;
		return;
	}

	unsigned long g_iModelBoneCounter = **(unsigned long**)(offs.invalidateBoneCache + 10);

	*(int*)((DWORD)player + offs.writeableBones) = 0;
	*(unsigned int*)((DWORD)player + 0x2914) = 0xFF7FFFFF; // m_flLastBoneSetupTime = -FLT_MAX;
	*(unsigned int*)((DWORD)player + 0x2680) = (g_iModelBoneCounter - 1); // m_iMostRecentModelBoneCounter = g_iModelBoneCounter - 1;

	player->setAbsOrigin(record.m_vAbsOrigin);
	*(Vector*)((DWORD)player + offs.vekVel) = record.m_vecVelocity;
	*(float*)((DWORD)player + offs.simulTime) = record.m_fSimulationTime;
	player->setAbsAngles(record.m_vecAngles);
	*(float*)((DWORD)player + offs.cycle) = record.m_flCycle;
	*(int*)((DWORD)player + offs.sequence) = record.m_nSequence;
	*(int*)((DWORD)player + offs.flags) = record.flags;
	*(float*)((DWORD)player + offs.lowBodYtarg) = record.m_flLowerBodyYawTarget;
	player->getCollideable()->vecMins() = record.m_vecMins;
	player->getCollideable()->vecMax() = record.m_vecMax;
	player->getFallVel() = record.m_fFallVel;

	//for (int i = 0; i < 24; i++)
	//	*(float*)((DWORD)player + offs.poseParams + sizeof(float) * i) = record.m_flPoseParameter[i];

	record.m_bIsFixed = true;

	p_Console->ConsoleColorPrintf(Color::LightBlue(), "backtracked %ls\n", player->getName(player->getIdx()).c_str());
}*/
/*int LagCompensation::fakeLagFix(CBaseEntity *player, int historyIdx)
{
	int idx = player->getIdx();
 
	LagRecord *m_LagRecords = this->m_LagRecord[idx];
 
	LagRecord recentLR = m_LagRecords[historyIdx];
	LagRecord prevLR;
	if (historyIdx == 0)
		prevLR = m_LagRecords[8];
	else
		prevLR = m_LagRecords[historyIdx - 1];
 
	if (recentLR.m_fSimulationTime == 0.0f)
		return -1;
 
	INetChannelInfo *nci = p_Engine->getNetChannelInfo();
	int predCmdArrivTick = Global::userCMD->tick_count + 1 + TIME_TO_TICKS(nci->GetAvgLatency(FLOW_INCOMING) + nci->GetAvgLatency(FLOW_OUTGOING)); // (c) n0xius @ uc
	
	int iLerpTicks = TIME_TO_TICKS(lerpTime());
	int iTargetTickCount = TIME_TO_TICKS(recentLR.m_fSimulationTime) + iLerpTicks;
 
	float flCorrect = clamp(lerpTime() + nci->GetLatency(FLOW_OUTGOING), 0.f, 1.f) - TICKS_TO_TIME(predCmdArrivTick + TIME_TO_TICKS(lerpTime()) - iTargetTickCount); // (c) n0xius @ uc
 
	if (fabs(flCorrect) > 0.2f) // Too big deltatime, can't lagfix here
	{
		p_Console->ConsoleColorPrintf(Color::Red(), "StartLagCompensation: delta too big (%.3f)\n", flCorrect);
		return -1;
	}
 
	if ((recentLR.m_vAbsOrigin - prevLR.m_vAbsOrigin).lengthSqr() > 4096.f)
	{
		for (auto i = 0; i <= player->getChockedTicks(); i++)
		{
			rebGameMovement->FullWalkMove(player); // resimulate
		}
		recentLR.m_bIsBreakingLagComp = true;
		p_Console->ConsoleColorPrintf(Color::Green(), "m_bIsBreakingLagComp: true\n");
 
		return -1;
	}
	else
	{
		setCBaseEntity(player, recentLR);
	}
 
	//p_Console->ConsoleColorPrintf(Color::LightBlue(), "tickcount: %d\n", (iTargetTickCount + 1));
 
	return iTargetTickCount /*+ 1*///wrong;/*
	//}*/

/*int LagCompensation::fixTickcount(CBaseEntity *player)
{
	int idx = player->getIdx();
 
	LagRecord *m_LagRecords = this->m_LagRecord[idx];
 
	LagRecord recentLR = m_LagRecords[mVars.historyIdx];
 
	if (recentLR.m_fSimulationTime == 0.0f)
		return TIME_TO_TICKS(player->getSimulTime() + lerpTime()) + 1;
	
	int iLerpTicks = TIME_TO_TICKS(lerpTime());
	int iTargetTickCount = TIME_TO_TICKS(recentLR.m_fSimulationTime) + iLerpTicks;
 
	p_Console->ConsoleColorPrintf(Color::Blue(), "tickcount: %d\n", (iTargetTickCount + 1));
 
	return iTargetTickCount;// wrong + 1;
	}*/