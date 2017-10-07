#include "sdk.h"
#include "Antiaim.h"
#include "Menu.h"
#include "global.h"
#include "Aimbot.h"
#include "GameUtils.h"
#include "Math.h"
#include "BacktrackingHelper.h"
CAntiaim* g_Antiaim = new CAntiaim;
float get3ddist2(Vector myCoords, Vector enemyCoords)
{
	return sqrt(
		pow(double(enemyCoords.x - myCoords.x), 2.0) +
		pow(double(enemyCoords.y - myCoords.y), 2.0) +
		pow(double(enemyCoords.z - myCoords.z), 2.0));
}
void AimAtPlayers()
{
	QAngle val;
	Vector eye_position = G::LocalPlayer->GetEyePosition();
	int value = G::UserCmd->viewangles.y, num = 0;
	float best_dist =G::MainWeapon->GetCSWpnData()->flRange;
	for (int i = 1; i < g_pGlobals->maxClients; ++i)
	{
		if (i == g_pEngine->GetLocalPlayer())
			continue;

		CBaseEntity* target = g_pEntitylist->GetClientEntity(i);
		player_info_t info;
		//Calls from left->right so we wont get an access violation error
		Vector pos;
		if (!target || /*target->IsDormant() ||*//*bc otherwise we get 1tapped by enemies throug dormant walls*/ target->GetHealth() < 1)
			continue;
		if ((G::LocalPlayer->GetTeamNum() == target->GetTeamNum() && RagebotElements.Ragebot_AtFriendly->Checked) || G::LocalPlayer->GetTeamNum() != target->GetTeamNum())
		{
			pos = target->GetEyePosition();
			float dist = get3ddist2(pos, G::LocalPlayer->GetEyePosition());
			if (RagebotElements.Ragebot_AntiAim_AtTargets->SelectedIndex == 1)
			{
				if (dist >= best_dist)
					continue;
			}
			best_dist = dist;

			QAngle angle = GameUtils::CalculateAngle(G::LocalPlayer->GetEyePosition(), pos);
			angle.y = Math::NormalizeYaw(angle.y);
			Math::ClampAngles(angle);

			if (RagebotElements.Ragebot_AntiAim_AtTargets->SelectedIndex == 1)
				value = angle.y;
			else
			{
				num++;
				value += angle.y;
			}
		}
	}
	if (RagebotElements.Ragebot_AntiAim_AtTargets->SelectedIndex == 2 && num != 0)
		value /= num;

	G::UserCmd->viewangles.y = value;
}
bool CAntiaim::EdgeAntiAim(float flWall, float flCornor)
{
	auto NormalizeVector = [](Vector& vec)
	{
		for (int i = 0; i < 3; ++i)
		{
			while (vec[i] > 180.f)
				vec[i] -= 360.f;

			while (vec[i] < -180.f)
				vec[i] += 360.f;
		}

		vec[2] = 0.f;
	};

	Ray_t ray;
	trace_t tr;

	CTraceFilter traceFilter;
	traceFilter.pSkip = G::LocalPlayer;

	auto bRetVal = false;
	auto vecCurPos = G::LocalPlayer->GetEyePosition();

	for (float i = 0; i < 360; i++)
	{
		Vector vecDummy(10.f, G::UserCmd->viewangles.y, 0.f);
		vecDummy.y += i;

		NormalizeVector(vecDummy);

		Vector vecForward;
		Math::AngleVectors(vecDummy, &vecForward);

		auto flLength = ((16.f + 3.f) + ((16.f + 3.f) * sin(DEG2RAD(10.f)))) + 7.f;
		vecForward *= flLength;

		ray.Init(vecCurPos, (vecCurPos + vecForward));
		g_pEngineTrace->TraceRay(ray, MASK_SHOT, &traceFilter, &tr);

		if (tr.fraction != 1.0f)
		{
			Vector qAngles;
			auto vecNegate = tr.plane.normal;

			vecNegate *= -1.f;
			Math::VectorAngles(vecNegate, qAngles);

			vecDummy.y = qAngles.y;

			NormalizeVector(vecDummy);
			trace_t leftTrace, rightTrace;

			Vector vecLeft;
			Math::AngleVectors(vecDummy + Vector(0.f, 30.f, 0.f), &vecLeft);

			Vector vecRight;
			Math::AngleVectors(vecDummy - Vector(0.f, 30.f, 0.f), &vecRight);

			vecLeft *= (flLength + (flLength * sin(DEG2RAD(30.f))));
			vecRight *= (flLength + (flLength * sin(DEG2RAD(30.f))));

			ray.Init(vecCurPos, (vecCurPos + vecLeft));
			g_pEngineTrace->TraceRay(ray, MASK_SHOT, &traceFilter, &leftTrace);

			ray.Init(vecCurPos, (vecCurPos + vecRight));
			g_pEngineTrace->TraceRay(ray, MASK_SHOT, &traceFilter, &rightTrace);

			if ((leftTrace.fraction == 1.f) && (rightTrace.fraction != 1.f))
				vecDummy.y -= flCornor; // left

			else if ((leftTrace.fraction != 1.f) && (rightTrace.fraction == 1.f))
				vecDummy.y += flCornor; // right	

			G::UserCmd->viewangles.x = 89.f;
			G::UserCmd->viewangles.y = vecDummy.y;
			G::UserCmd->viewangles.y -= flWall;

			if (RagebotElements.Ragebot_AntiAim_Edge->SelectedIndex == 2 || RagebotElements.Ragebot_AntiAim_Edge->SelectedIndex == 3)
			{
				if (G::SendPacket || (this->lby_update_in_this_tick && RagebotElements.Ragebot_AntiAim_Edge->SelectedIndex == 3))
					G::UserCmd->viewangles.y -= 180.f;
			}
			bRetVal = true;
		}
	}
	return bRetVal;
}
void CAntiaim::Pitch_AA()
{
	int Type = RagebotElements.Ragebot_AntiAim_Pitch->SelectedIndex;
	static bool mixed;
	float temp = 0;
	/*
	emotion = 89
down = 178
fake down = 180.001
up = -89
fake up = 991
zero = 0
fake zero = 1080
mixed = 89 / -89 switch
custom is custom*/
	switch (Type)
	{
	case 1:
		temp = 89; //Emotion
		break;

	case 2:
		temp = 179.f; //Down
		break;
	case 3:
		temp = -89;
		break;

	case 4:
		temp = 180.001f; //fakedown
		break;
	case 5:
		mixed = !mixed;
		if (mixed)
			temp = 89;
		else
			temp = -89;
		break;
	case 6:
		temp = -991;
		break;
	case 7: 
		temp = 991;
		break;
	case 8:
		temp = 1080;
		break;
	case 9:
		temp = -1080;
		break;
	}
	G::UserCmd->viewangles.x = temp;
}
void CAntiaim::Yaw_AA()
{
	float temp = G::UserCmd->viewangles.y;


	int Type = RagebotElements.Ragebot_AntiAim_Yaw->SelectedIndex;
	
	int random = rand() % 100;

	static bool swap;
	if (G::SendPacket)
		swap = !swap;

	if (Type == 1)
	{
		if (G::SendPacket)
			temp += 90.f;
		else
			if (swap)
				temp -= 180.f;
			else
				temp -= 90.f;
	}
	else if (Type == 2)
	{
		if (G::SendPacket)
			if (swap)
				temp += 90.f;
			else
				temp -= 90.f;
		else
		{

			if (G::UserCmd->command_number % 3)
				temp -= 180.f;
			else
			{
				if (swap)
					temp -= 165;
				else
					temp += 165;
			}
		}
	}
	else if (Type == 3)
	{
		if (G::SendPacket)
			if (swap)
				temp -= 165;
			else
				temp += 165;
		else
			if (swap)
				temp += 165;
			else
				temp -= 165;
	}
	else if (Type == 4)
	{
		static int step = 0;
		if (G::SendPacket)
			step += 1;
		if (step > 3)
			step = 0;

		static int currentstepyaw;

		if (G::SendPacket)
		{
			switch (step)
			{
			case 0:
				temp = 45;
				break;
			case 1:
				temp = 225;
				break;
			case 2:
				temp = 315;
				break;
			case 3:
				temp = 135;
				break;
			default:
				break;
			}
			currentstepyaw = temp;
		}
		else
		{
			static bool intern_swap;
			intern_swap = !intern_swap;
			if (G::UserCmd->command_number % 3)
				temp -= 180.f;
			else
			{
				if (intern_swap)
					temp = currentstepyaw + 90.f;
				else
					temp = currentstepyaw - 90.f;
			}
		}
	}
	else if (Type == 5)
	{
			temp = G::LocalPlayer->LowerBodyYaw() + 180;
	}
	G::UserCmd->viewangles.y = temp;
}
void CAntiaim::AntiAimYaw_Real()
{
	float jitterrange = RagebotElements.Ragebot_AntiAim_Custom_Rand_Jitter->Checked ? Math::RandomFloat(0, RagebotElements.Ragebot_AntiAim_Custom_Jitter_Range->value) : RagebotElements.Ragebot_AntiAim_Custom_Jitter_Range->value;

	static bool flip = false;
	static int swap = 0;
	swap++;
	if (swap >= 3)
	{
		swap = 0;
		flip = !flip;
	}

	float temp = G::UserCmd->viewangles.y;

	int Type = RagebotElements.Ragebot_AntiAim_Yaw_Real->SelectedIndex;


	int random = rand() % 100;
	
	if (Type == 1)
	{
		auto DegreesToRadians = [](float Angle)
		{
			return Angle * M_PI / 180.0f;
		};
		temp += fmod(DegreesToRadians(g_pGlobals->framecount * g_pGlobals->curtime), 360);
		temp = Math::NormalizeYaw(temp);
	}
	else if (Type == 2)
	{
		static QAngle Angle;
		Angle.y += 15;
		Angle.y = Math::NormalizeYaw(Angle.y);
		temp = Angle.y;
	}
	else if (Type == 3)
	{
		temp -= 180.f;
		temp += flip ? jitterrange : -jitterrange;
	}
	else if (Type == 4)
	{
		temp -= 180.f;
	}
	else if (Type == 5)
	{
		if (G::LocalPlayer->GetVelocity().Length() > 0.1)
			temp += 180.f;
		else
			temp += this->lby_update_in_this_tick ? 90.f:  -90.f;
	}
	else if (Type == 6)
	{
		static float add = 0;
		static bool reset = true;

		if (reset)
		{
			add = 0;
			reset = false;
		}

		float temp_base = temp;

		temp += 135;
		temp += add;

		add += 15;

		if (temp_base + 225 < temp)
		{
			reset = true;
			temp = temp_base + 225;
		}
	}
	else if (Type == 7)
	{
		//real
		int mode = RagebotElements.Ragebot_AntiAim_Custom_Mode->SelectedIndex;
		if (!RagebotElements.Ragebot_AntiAim_Custom_Dynamic->Checked)
			temp = 0;

		float jitterrange = RagebotElements.Ragebot_AntiAim_Custom_Rand_Jitter->Checked ? Math::RandomFloat(0, RagebotElements.Ragebot_AntiAim_Custom_Jitter_Range->value) : RagebotElements.Ragebot_AntiAim_Custom_Jitter_Range->value;

		switch (mode)
		{
		case 0:
			temp += RagebotElements.Ragebot_AntiAim_Custom_Real_Yaw->value;
			break;
		case 1:
			temp += RagebotElements.Ragebot_AntiAim_Custom_Real_Yaw->value;
			temp += flip ? jitterrange : -(jitterrange);
			break;

		default:
			break;
		}
	}
	G::UserCmd->viewangles.y = temp;
}
void CAntiaim::AntiAimYaw_Fake()
{
	static bool flip = false;
	static int swap = 0;
	swap++;
	if (swap >= 3)
	{
		swap = 0;
		flip = !flip;
	}

	

	float jitterrange = RagebotElements.Ragebot_AntiAim_Custom_Rand_Jitter->Checked ? Math::RandomFloat(0, RagebotElements.Ragebot_AntiAim_Custom_Jitter_Range->value) : RagebotElements.Ragebot_AntiAim_Custom_Jitter_Range->value;

	float temp = G::UserCmd->viewangles.y;



	int Type = RagebotElements.Ragebot_AntiAim_Yaw_Fake->SelectedIndex;

	int random = rand() % 100;
	
	if (Type == 1)
	{
		auto DegreesToRadians = [](float Angle)
		{
			return Angle * M_PI / 180.0f;
		};
		temp += fmod(DegreesToRadians(g_pGlobals->framecount * g_pGlobals->curtime), 360);
		temp = Math::NormalizeYaw(temp);
	}
	else if (Type == 2)
	{
		static QAngle Angle;
		Angle.y += 15;
		Angle.y = Math::NormalizeYaw(Angle.y);
		temp = Angle.y;
	}
	else if (Type == 3)
	{
		temp -= 180.f;
		temp += flip ? jitterrange : -jitterrange;
	}
	else if (Type == 4)
	{
		temp += 90.f;
	}
	else if (Type == 5)
	{
		static float add = 0;
		static bool reset = true;

		if (reset)
		{
			add = 0;
			reset = false;
		}

		float temp_base = temp;

		temp += 135;
		temp += add;

		add += 15;

		if (temp_base + 225 < temp)
		{
			reset = true;
			temp = temp_base + 225;
		}
	}
	else if (Type == 6)
	{
		//real
		int mode = RagebotElements.Ragebot_AntiAim_Custom_Mode->SelectedIndex;
		if (!RagebotElements.Ragebot_AntiAim_Custom_Dynamic->Checked)
			temp = 0;

		

		switch (mode)
		{
		case 0:
			temp += RagebotElements.Ragebot_AntiAim_Custom_Fake_Yaw->value;
			break;
		case 1:
			temp += RagebotElements.Ragebot_AntiAim_Custom_Fake_Yaw->value;
			temp += flip ? -jitterrange : (jitterrange);
			break;
	
			
		default:
			break;
		}
	}
	G::UserCmd->viewangles.y = temp;
}
bool GetAlive(CBaseEntity* pLocal)
{
	for (int i = 1; i < 64; ++i)
	{
		if (i == g_pEngine->GetLocalPlayer())
			continue;

		CBaseEntity* target = g_pEntitylist->GetClientEntity(i);
		player_info_t info;
		//Calls from left->right so we wont get an access violation error
		Vector pos;
		if (!target ||  target->GetHealth() < 1)
			continue;
		if (pLocal->GetTeamNum() != target->GetTeamNum())
		{
			return true;
		}
	}
	return false;
}
void CAntiaim::Run(QAngle org_view)
{
	if (RagebotElements.Ragebot_AntiAim_Active->Checked)
	{
		static int iChokedPackets = -1;

		if ((g_Aimbot->fired_in_that_tick && iChokedPackets < 4 && GameUtils::IsAbleToShoot()) && !G::ForceRealAA)
		{
			G::SendPacket = false;
			iChokedPackets++;
		}
		else
		{
			iChokedPackets = 0;

			bool anyaliveplayers = GetAlive(G::LocalPlayer);
			if (!anyaliveplayers && RagebotElements.Ragebot_AntiAim_Disable_NoPlayers->Checked)
				return;
			if (G::UserCmd->buttons & IN_USE)
				return;
			if (G::MainWeapon->IsGrenade())
			{
				CGrenade* pGrenade = (CGrenade*)G::MainWeapon;
				if (pGrenade)
				{
					if (!pGrenade->IsPinPulled() || G::UserCmd->buttons & IN_ATTACK || G::UserCmd->buttons & IN_ATTACK2)
					{

						float fThrowTime = pGrenade->GetThrowTime();

						if ((fThrowTime > 0) && (fThrowTime < g_pGlobals->curtime))
							return;
					}
				}
			}
			else
			{
				if (G::MainWeapon->IsMiscWeapon() && RagebotElements.Ragebot_AntiAim_Disable_Knife->Checked)
					return;
			}

			choke = !choke;
			if (MiscElements.Misc_Movement_FakelagType->SelectedIndex == 0 || (G::LocalPlayer->GetFlags() & FL_ONGROUND && !MiscElements.Misc_Movement_Fakelag_OnGround->Checked))
				G::SendPacket = choke;

			//update lby shit
			this->lby_update_in_this_tick = false;
			float latency = g_pEngine->GetNetChannelInfo()->GetLatency(FLOW_OUTGOING);
			float flCurTime = g_pGlobals->curtime;
			static float flNextTimeUpdate = 0;// flCurTime + 1.1f;


			static float time_between = 0;
			if (G::LocalPlayer->GetVelocity().Length() == 0)
			{
				if (flNextTimeUpdate +/*-*/ latency <= flCurTime || flNextTimeUpdate - flCurTime > 2.f)
				{

					this->lby_update_in_this_tick = true;
					G::SendPacket = false;

					time_between = g_pGlobals->curtime;

					//if (G::LocalPlayer->GetVelocity().Length() > 0.1)
						//flNextTimeUpdate = flCurTime + 0.22000001f;
					//else
					flNextTimeUpdate = flCurTime + 1.1f;
				}
			}
			if (RagebotElements.Ragebot_AntiAim_Edge->SelectedIndex > 0)
			{


				bool bEdge = EdgeAntiAim(0.f, 89.f);

				if (bEdge)
					return;
			}

			G::UserCmd->viewangles = org_view; //fixes aimbot angles
			if (RagebotElements.Ragebot_AntiAim_AtTargets->SelectedIndex > 0)
			{
				AimAtPlayers();

			}
		
			

				

				Pitch_AA();
				if (G::SendPacket)
					AntiAimYaw_Fake();
				else
				{
					//if ((G::LocalPlayer->GetFlags() & FL_ONGROUND))
					//	AntiAimYaw_Fake();
					//else
						AntiAimYaw_Real();
				}
		}
	}
}