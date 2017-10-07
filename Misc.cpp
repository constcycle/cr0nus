#include "sdk.h"
#include "Misc.h"
#include "global.h"
#include <chrono>
#include "GameUtils.h"
#include "Menu.h"
#include "Math.h"
#include "xor.h"
#include <chrono>
CMisc* g_Misc = new CMisc;
//using RevealAllFn = void(*)(int); Double V fix: Can't use this syntax with my VS version!
typedef void(*RevealAllFn)(int);
RevealAllFn fnReveal;
void CMisc::RankReveal()
{
	if (!MiscElements.Misc_General_RevealRanks->Checked)
		return;

	if(!fnReveal)
		fnReveal = (RevealAllFn)Utilities::Memory::FindPattern(XorStr("client.dll"), (PBYTE)XorStr("\x55\x8B\xEC\x8B\x0D\x00\x00\x00\x00\x68\x00\x00\x00\x00"), XorStr("xxxxx????x????"));

	int iBuffer[1];

	if (G::UserCmd->buttons & IN_SCORE)
		fnReveal(iBuffer[1]);
}

void CMisc::Bunnyhop()
{
	if (MiscElements.Misc_Movement_Bunnyhop->Checked/* && Cvar->FindVar("sv_enablebunnyhopping")->GetValue() != 1*/)
	{
		static auto bJumped = false;
		static auto bFake = false;
		if (!bJumped && bFake)
		{
			bFake = false;
			G::UserCmd->buttons |= IN_JUMP;
		}
		else if (G::UserCmd->buttons & IN_JUMP)
		{
			if (G::LocalPlayer->GetFlags() & FL_ONGROUND)
			{
				bJumped = true;
				bFake = true;
			}
			else
			{
				G::UserCmd->buttons &= ~IN_JUMP;
				bJumped = false;
			}
		}
		else
		{
			bJumped = false;
			bFake = false;
		}
	}
}
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
#define CheckIfNonValidNumber(x) (fpclassify(x) == FP_INFINITE || fpclassify(x) == FP_NAN || fpclassify(x) == FP_SUBNORMAL)
void CMisc::AutoStrafe()
{
	if (!MiscElements.Misc_Movement_Autostrafe->Checked)
		return;

	if (!GetAsyncKeyState(VK_SPACE))
		return;

	/* AW*/
	static int old_yaw;

	auto get_velocity_degree = [](float length_2d)
	{
	auto tmp = RAD2DEG(atan(30.f / length_2d));

	if (CheckIfNonValidNumber(tmp) || tmp > 90.f)
	return 90.f;

	else if (tmp < 0.f)
	return 0.f;

	else
	return tmp;
	};

	//if (pLocal->GetMoveType() != MOVETYPE_WALK)
	//return;

	auto velocity = G::LocalPlayer->GetVelocity();
	velocity.z = 0;

	static auto flip = false;
	auto turn_direction_modifier = (flip) ? 1.f : -1.f;
	flip = !flip;

	if (G::LocalPlayer->GetFlags() & FL_ONGROUND || G::LocalPlayer->GetMoveType() == MOVETYPE_LADDER)
		return;

	if (G::UserCmd->forwardmove > 0.f)
		G::UserCmd->forwardmove = 0.f;

	auto velocity_length_2d = velocity.Length2D();

	auto strafe_angle = RAD2DEG(atan(15.f / velocity_length_2d));

	if (strafe_angle > 90.f)
	strafe_angle = 90.f;

	else if (strafe_angle < 0.f)
	strafe_angle = 0.f;

	Vector Buffer(0, G::StrafeAngle.y - old_yaw, 0);
	Buffer.y = Math::NormalizeYaw(Buffer.y);

	int yaw_delta = Buffer.y;
	old_yaw = G::StrafeAngle.y;

	if (yaw_delta > 0.f)
		G::UserCmd->sidemove = -450.f;

	else if (yaw_delta < 0.f)
		G::UserCmd->sidemove = 450.f;

	auto abs_yaw_delta = abs(yaw_delta);

	if (abs_yaw_delta <= strafe_angle || abs_yaw_delta >= 30.f)
	{
	Vector velocity_angles;
	Math::VectorAngles(velocity, velocity_angles);

	Buffer = Vector(0, G::StrafeAngle.y - velocity_angles.y, 0);
	Buffer.y = Math::NormalizeYaw(Buffer.y);
	int velocityangle_yawdelta = Buffer.y;

	auto velocity_degree = get_velocity_degree(velocity_length_2d) * MiscElements.Misc_Movement_Strafe_Retrack->value; // retrack value, for teleporters

	if (velocityangle_yawdelta <= velocity_degree || velocity_length_2d <= 15.f)
	{
	if (-(velocity_degree) <= velocityangle_yawdelta || velocity_length_2d <= 15.f)
	{
		G::StrafeAngle.y += (strafe_angle * turn_direction_modifier);
	G::UserCmd->sidemove = 450.f * turn_direction_modifier;
	}

	else
	{
		G::StrafeAngle.y = velocity_angles.y - velocity_degree;
		G::UserCmd->sidemove = 450.f;
	}
	}

	else
	{
		G::StrafeAngle.y = velocity_angles.y + velocity_degree;
		G::UserCmd->sidemove = -450.f;
	}


	}




	G::UserCmd->buttons &= ~(IN_MOVELEFT | IN_MOVERIGHT | IN_FORWARD | IN_BACK);

	if (G::UserCmd->sidemove <= 0.0)
		G::UserCmd->buttons |= IN_MOVELEFT;
	else
		G::UserCmd->buttons |= IN_MOVERIGHT;

	if (G::UserCmd->forwardmove <= 0.0)
		G::UserCmd->buttons |= IN_BACK;
	else
		G::UserCmd->buttons |= IN_FORWARD;

/*SKEET & CSTRAFE*/
	/*auto get_angle_from_speed = [](float speed)
	{
		auto ideal_angle = RAD2DEG(std::atan2(30.f, speed));
		ideal_angle = clamp<float>(ideal_angle, 0.f, 90.f);
		return ideal_angle;
	};

	auto get_velocity_step = [](Vector velocity, float speed, float circle_yaw)
	{
		auto velocity_degree = RAD2DEG(std::atan2(velocity.x, velocity.y));
		auto step = 1.5f;

		Vector start = G::LocalPlayer->GetOrigin(), end = G::LocalPlayer->GetOrigin();

		Ray_t ray;
		CGameTrace trace;
		CTraceFilter filter;

		while (true)
		{
			end.x += (std::cos(DEG2RAD(velocity_degree + circle_yaw)) * speed);
			end.y += (std::sin(DEG2RAD(velocity_degree + circle_yaw)) * speed);
			end *= g_pGlobals->frametime;

			ray.Init(start, end, Vector(-20.f, -20.f, 0.f), Vector(20.f, 20.f, 32.f));
			g_pEngineTrace->TraceRay(ray, CONTENTS_SOLID, &filter, &trace);

			if (trace.fraction < 1.f || trace.allsolid || trace.startsolid)
				break;

			step -= g_pGlobals->frametime;

			if (step == 0.f)
				break;

			start = end;
			velocity_degree += (velocity_degree + circle_yaw);
		}

		return step;
	};

	auto set_button_state = [](CUserCmd* cmd)
	{
		cmd->buttons &= ~(IN_MOVELEFT | IN_MOVERIGHT | IN_FORWARD | IN_BACK);

		if (cmd->sidemove <= 0.0)
			cmd->buttons |= IN_MOVELEFT;
		else
			cmd->buttons |= IN_MOVERIGHT;

		if (cmd->forwardmove <= 0.0)
			cmd->buttons |= IN_BACK;
		else
			cmd->buttons |= IN_FORWARD;
	};

	//if ( !menu->main.misc.air_strafer.checked( ) || opulence->me->m_MoveType( ) != MOVETYPE_WALK || opulence->me->m_fFlags( ) & FL_ONGROUND )
	//return;
	if (G::LocalPlayer->GetFlags() & FL_ONGROUND || G::LocalPlayer->GetMoveType() == MOVETYPE_LADDER)
		return;

	auto velocity = G::LocalPlayer->GetVelocity();
	velocity.z = 0;

	static bool flip = false;
	auto turn_direction_modifier = flip ? 1.f : -1.f;
	flip = !flip;

	if (G::UserCmd->forwardmove > 0.f)
		G::UserCmd->forwardmove = 0.f;

	auto speed = velocity.Length2D();

	static float m_circle_yaw;
	static float m_previous_yaw;
	// circle strafe
	/*if (MiscElements.Misc_Movement_Circlestrafe->Checked && GetAsyncKeyState(MiscElements.Misc_Movement_Circlestrafe_Key->Key))
	{
		auto ideal_speed_angle = get_angle_from_speed(speed);

		m_circle_yaw = Math::NormalizeYaw(m_circle_yaw + ideal_speed_angle);
		auto step = get_velocity_step(velocity, speed, ideal_speed_angle);

		if (step != 0.f)
			m_circle_yaw += (((g_pGlobals->frametime * 128.f) * step) * step);

		G::StrafeAngle.y = Math::NormalizeYaw(m_circle_yaw);
		G::UserCmd->sidemove = -450.f;
		set_button_state(G::UserCmd);
		return;
	}*/
/*

	auto ideal_move_angle = RAD2DEG(std::atan2(15.f, speed));
	ideal_move_angle = clamp<float>(ideal_move_angle, 0.f, 90.f);

	auto yaw_delta = Math::NormalizeYaw(G::StrafeAngle.y - m_previous_yaw);
	auto abs_yaw_delta = abs(yaw_delta);
	m_circle_yaw = m_previous_yaw = G::StrafeAngle.y;

	if (yaw_delta > 0.f)
		G::UserCmd->sidemove = -450.f;

	else if (yaw_delta < 0.f)
		G::UserCmd->sidemove = 450.f;

	if (abs_yaw_delta <= ideal_move_angle || abs_yaw_delta >= 30.f)
	{
		Vector velocity_angles;
		Math::VectorAngles(velocity, velocity_angles);

		auto velocity_angle_yaw_delta = Math::NormalizeYaw(G::StrafeAngle.y - velocity_angles.y);
		auto velocity_degree = get_angle_from_speed(speed) * MiscElements.Misc_Movement_Strafe_Retrack->value;

		if (velocity_angle_yaw_delta <= velocity_degree || speed <= 15.f)
		{
			if (-(velocity_degree) <= velocity_angle_yaw_delta || speed <= 15.f)
			{
				G::StrafeAngle.y += (ideal_move_angle * turn_direction_modifier);
				G::UserCmd->sidemove = 450.f * turn_direction_modifier;
			}

			else
			{
				G::StrafeAngle.y = velocity_angles.y - velocity_degree;
				G::UserCmd->sidemove = 450.f;
			}
		}

		else
		{
			G::StrafeAngle.y = velocity_angles.y + velocity_degree;
			G::UserCmd->sidemove = -450.f;
		}
	}

	set_button_state(G::UserCmd);

	*/
}

void CMisc::FixCmd()
{
	if (MiscElements.Misc_General_Restriction->Checked)
	{

		G::UserCmd->viewangles.y = Math::NormalizeYaw(G::UserCmd->viewangles.y);
		Math::ClampAngles(G::UserCmd->viewangles);

		if (G::UserCmd->forwardmove > 450)
			G::UserCmd->forwardmove = 450;
		if (G::UserCmd->forwardmove < -450)
			G::UserCmd->forwardmove = -450;

		if (G::UserCmd->sidemove > 450)
			G::UserCmd->sidemove = 450;
		if (G::UserCmd->sidemove < -450)
			G::UserCmd->sidemove = -450;
	}
}
bool CMisc::FakeLag()
{
	if (G::LocalPlayer->GetFlags() & FL_ONGROUND)
	{
		if (!MiscElements.Misc_Movement_Fakelag_OnGround->Checked)
		{
			G::SendPacket = true;
			return false;
		}
	}
	
	//hardcore
	int Type = MiscElements.Misc_Movement_FakelagType->SelectedIndex;

	static int ticks = 0;
	const int ticksMax = 14; //16

	static bool new_factor = false;

	static int packetsToChoke = 0;

	if (Type == 1)
	{
		packetsToChoke = MiscElements.Misc_Movement_Fakelag_Choke->value;
	}
	else if (Type == 2)
	{
		float z_velocity = G::LocalPlayer->GetVelocity().z * 3; //5.5 is the peak
		if (z_velocity < 0)
			z_velocity *= -1;

		

		packetsToChoke = z_velocity;
		if (packetsToChoke == 0)
			packetsToChoke += 3;
	}
	else if (Type == 3)
	{
		packetsToChoke = max(1, min((int)(fabs(G::LocalPlayer->GetVelocity().Length() / 80.f)), 5));
	}
	else if (Type == 4 && new_factor)
	{
		packetsToChoke = (rand() % 8) + 6;
		new_factor = false;
	}
	else if (Type == 5 && new_factor)
	{
		static int FakelagFactor = 10;
		static int m_iOldHealth = 0;
		static int iChoked;
		if (m_iOldHealth != G::LocalPlayer->GetHealth())
		{
			m_iOldHealth = G::LocalPlayer->GetHealth();
			packetsToChoke = rand() % 7 + 6 + 1;
			new_factor = false;
		}
	}
	else if (Type == 6 && new_factor)
	{
		int min_packets = 0;
		Vector vel_p_t = Vector(G::LocalPlayer->GetVelocity() * g_pGlobals->interval_per_tick);
		for (int i = 1; i <= 14; i++)
		{
			if (Vector(vel_p_t * i).Length() > 64)
			{
				min_packets = i;
				break;
			}
		}

		packetsToChoke = int(Math::RandomFloat(min_packets, ticksMax));
	}

	if (packetsToChoke > ticksMax)
		packetsToChoke = ticksMax;

	if (ticks >= packetsToChoke)
	{
		ticks = 0;
		G::SendPacket = true;
		new_factor = true;
	}
	else
	{
		G::SendPacket = false;
	}


	ticks++;

	return false;
}
void CMisc::FixMovement()
{
	Vector vMove =Vector(G::UserCmd->forwardmove, G::UserCmd->sidemove, 0.0f);
	float flSpeed = vMove.Length();
	Vector qMove;
	Math::VectorAngles(vMove, qMove);
	float normalized = fmod(G::UserCmd->viewangles.y + 180.f, 360.f) - 180.f;
	float normalizedx = fmod(G::UserCmd->viewangles.x + 180.f, 360.f) - 180.f;
	float flYaw = DEG2RAD((normalized - G::StrafeAngle.y) + qMove.y);

	if (normalizedx >= 90.0f || normalizedx <= -90.0f || (G::UserCmd->viewangles.x >= 90.f && G::UserCmd->viewangles.x <= 200) || G::UserCmd->viewangles.x <= -90)
		G::UserCmd->forwardmove = -cos(flYaw) * flSpeed;
	else 
		G::UserCmd->forwardmove = cos(flYaw) * flSpeed;

	G::UserCmd->sidemove = sin(flYaw) * flSpeed;
}


void LoadPathing()
{

	char path[MAX_PATH];
	GetModuleFileNameA(GetModuleHandle(NULL), path, 255);
	for (int i = strlen(path); i > 0; i--)
	{
		if (path[i] == '\\')
		{
			path[i + 1] = 0;
			break;
		}
	}

	char size[8];
	char vecstr[64];
	char itostr[8];
	CBaseEntity *pLocal = G::LocalPlayer;
	if (!pLocal)
		return;
	if (g_Misc->path.size() > 1)
	{

		g_Misc->path.erase(g_Misc->path.begin(), g_Misc->path.end());
		sprintf_s(path, "\\%s_%s_%d.cfg", "pathes", "test"/*g_pEngine->GetLevelName()*/, pLocal->GetTeamNum());
		GetPrivateProfileStringA("Points", "Size", "0", size, 8, path);
		int numPoints = atoi(size);
		for (int i = 0; i < numPoints - 1; i++)
		{
			char vecstr[64];
			char itostr[8];
			sprintf_s(itostr, "%d", i);
			GetPrivateProfileStringA("Pathing", itostr, "0.0 0.0 0.0", vecstr, 64, path);
			std::string PosStr = vecstr;
			string buffer;
			stringstream ss(PosStr);
			vector<string> floats;
			while (ss >> buffer)
				floats.push_back(buffer);

			g_Misc->path.push_back(Vector(stof(floats[0]), stof(floats[1]), stof(floats[2])));
		}
	}
}

void SavePathing()
{

	char path[MAX_PATH];
	GetModuleFileNameA(GetModuleHandle(NULL), path, 255);
	for (int i = strlen(path); i > 0; i--)
	{
		if (path[i] == '\\')
		{
			path[i + 1] = 0;
			break;
		}
	}
	char size[8];
	char vecstr[64];
	char itostr[8];

	if (g_Misc->path.size() > 1)
	{
		sprintf_s(path, "\\%s_%s_%d.cfg", "pathes", "test"/*g_pEngine->GetLevelName()*/, G::LocalPlayer->GetTeamNum());
		printf("Path %s\n", path);
		sprintf_s(size, "%d", g_Misc->path.size() + 1);
		WritePrivateProfileStringA("Points", "Size", size, path);
		for (int i = 0; i < g_Misc->path.size(); i++)
		{
			sprintf_s(itostr, "%d", i);
			sprintf_s(vecstr, "%f %f %f", g_Misc->path.at(i).x, g_Misc->path.at(i).y, g_Misc->path.at(i).z);
			WritePrivateProfileStringA("Pathing", itostr, vecstr, path);
		}
	}
}
bool MarksIsVisible(CBaseEntity* local, Vector& vTo) 
{
	Ray_t ray;
	trace_t trace;
	CTraceFilterNoPlayer filter;
	filter.pSkip = local;

	ray.Init(local->GetEyePosition(), vTo);
	g_pEngineTrace->TraceRay(ray, 0x4600400B, &filter, &trace);
	return (trace.fraction > 0.99f);
}