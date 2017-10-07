#include "sdk.h"
#include "Autowall.h"
#include "GameUtils.h"
#include "Math.h"
#include "Menu.h"
#include "global.h"
CAutowall* g_Autowall = new CAutowall;

#define    HITGROUP_GENERIC    0
#define    HITGROUP_HEAD        1
#define    HITGROUP_CHEST        2
#define    HITGROUP_STOMACH    3
#define HITGROUP_LEFTARM    4    
#define HITGROUP_RIGHTARM    5
#define HITGROUP_LEFTLEG    6
#define HITGROUP_RIGHTLEG    7
#define HITGROUP_GEAR        10

/*
$qch: oh ye you will
$qch: like handlebulletpen doesn't check for the hitbox or something
$qch: it only check for materials
$qch: between you and point
D3dC0m: But the damage will always be 0
$qch: no
$qch: it's based of you current weapon damage
$qch: than it check for the materials and lower damage based on this
$qch: then scale based of hitbox group
$qch: All you will have to fix
D3dC0m: OH
D3dC0m: Use a traceray
$qch: is that you will need to manually pass hitgroup
D3dC0m: That ignores eveerything
D3dC0m: than players
D3dC0m: except*
D3dC0m: to get hitgroup
$qch: ye
$qch: I made a func to dump hitgroup based of hitbox
$qch: so now I have a func hitbox to hitgroup
D3dC0m: ye

*/

float GetHitgroupDamageMult(int iHitGroup)
{
	switch (iHitGroup)
	{
	case HITGROUP_GENERIC:
		return 1.0f;
	case HITGROUP_HEAD:
		return 2.0f;
	case HITGROUP_CHEST:
		return 1.0f;
	case HITGROUP_STOMACH:
		return 1.5f;
	case HITGROUP_LEFTARM:
		return 1.0f;
	case HITGROUP_RIGHTARM:
		return 1.0f;
	case HITGROUP_LEFTLEG:
		return 0.75f;
	case HITGROUP_RIGHTLEG:
		return 0.75f;
	case HITGROUP_GEAR:
		return 1.0f;
	default:
		break;

	}

	return 1.0f;
}

void ScaleDamage(int hitgroup, CBaseEntity *enemy, float weapon_armor_ratio, float &current_damage)
{
	current_damage *= GetHitgroupDamageMult(hitgroup);

	if (enemy->GetArmor() > 0)
	{
		if (hitgroup == HITGROUP_HEAD)
		{
			if (enemy->HasHelmet())
				current_damage *= weapon_armor_ratio;
		}
		else
		{
			current_damage *= weapon_armor_ratio;
		}
	}
}
//Old autowall
DWORD dwClipTraceToPlayer;
void UTIL_ClipTraceToPlayers(const Vector& vecAbsStart, const Vector& vecAbsEnd, unsigned int mask, ITraceFilter* filter, trace_t* tr)
{
	
	if (!dwClipTraceToPlayer)
		dwClipTraceToPlayer = FindPatternIDA("client.dll", "53 8B DC 83 EC 08 83 E4 F0 83 C4 04 55 8B 6B 04 89 6C 24 04 8B EC 81 EC ? ? ? ? 8B 43 10");


	_asm
	{
		MOV		EAX, filter
		LEA		ECX, tr
		PUSH	ECX
		PUSH	EAX
		PUSH	mask
		LEA		EDX, vecAbsEnd
		LEA		ECX, vecAbsStart
		CALL	dwClipTraceToPlayer
		ADD		ESP, 0xC
	}
}
DWORD dwEntBreakable = NULL;
bool TraceToExit(Vector& vecEnd, trace_t* pEnterTrace, Vector vecStart, Vector vecDir, trace_t* pExitTrace)
{
	auto distance = 0.0f;
	auto DidHitNonWorldCBaseEntity = [](CBaseEntity* m_pEnt) -> bool
	{
		return m_pEnt != nullptr && m_pEnt == g_pEntitylist->GetClientEntity(0);
	};

	while (distance <= 90.0f)
	{
		distance += 4.0f;
		vecEnd = vecStart + vecDir * distance;

		auto point_contents = g_pEngineTrace->GetPointContents(vecEnd, MASK_SHOT_HULL | CONTENTS_HITBOX, nullptr);
		if (point_contents & MASK_SHOT_HULL && (!(point_contents & CONTENTS_HITBOX)))
			continue;

		auto new_end = vecEnd - (vecDir * 4.0f);

		Ray_t ray;
		ray.Init(vecEnd, new_end);
		g_pEngineTrace->TraceRay(ray, 0x4600400B, nullptr, pExitTrace);

		if (pExitTrace->m_pEnt == nullptr)
			return false;

		if (pExitTrace->startsolid && pExitTrace->surface.flags & SURF_HITBOX)
		{
			CTraceFilter filter;
			filter.pSkip = pExitTrace->m_pEnt;

			ray.Init(vecEnd, vecStart);
			g_pEngineTrace->TraceRay(ray, 0x600400B, &filter, pExitTrace);

			if ((pExitTrace->fraction < 1.0f || pExitTrace->allsolid) && !pExitTrace->startsolid)
			{
				vecEnd = pExitTrace->endpos;
				return true;
			}

			continue;
		}

		if (!(pExitTrace->fraction < 1.0 || pExitTrace->allsolid || pExitTrace->startsolid) || pExitTrace->startsolid)
		{
			if (pExitTrace->m_pEnt)
			{
		
				/*disable the FindPattern shit and stuff and use the rebuilt one, once glass doesn't crash anymore*/
				CBaseEntity* pBreakEnt = pExitTrace->m_pEnt;
				
				if (dwEntBreakable == NULL)
				{
					dwEntBreakable = FindPatternIDA("client.dll", "55 8B EC 51 56 8B F1 85 F6 74 68");
				}
				bool bBreakable;

				__asm
				{
					MOV ECX, pBreakEnt
					CALL dwEntBreakable
					MOV bBreakable, AL
				}
				

				if (!DidHitNonWorldCBaseEntity(pExitTrace->m_pEnt) && /*GameUtils::IsBreakableEntity(pExitTrace->m_pEnt) &&*/ bBreakable)
					return true;
			}

			continue;
		}

		if (((pExitTrace->surface.flags >> 7) & 1) && !((pExitTrace->surface.flags >> 7) & 1))
			continue;

		if (pExitTrace->plane.normal.Dot(vecDir) < 1.1f)
		{
			float fraction = pExitTrace->fraction * 4.0f;
			vecEnd = vecEnd - (vecDir * fraction);

			return true;
		}
	}

	return false;
}

bool CAutowall::HandleBulletPenetration(CSWeaponInfo* wpn_data, FireBulletData& data)
{
	surfacedata_t *enter_surface_data = g_pPhysics->GetSurfaceData(data.enter_trace.surface.surfaceProps);

	int enter_material = enter_surface_data->game.material;

	float enter_surf_penetration_mod = enter_surface_data->game.flPenetrationModifier;


	data.trace_length += data.enter_trace.fraction * data.trace_length_remaining;
	data.current_damage *= (float)pow(wpn_data->flRangeModifier, (data.trace_length * 0.002));

	if ((data.trace_length > 3000.f) || (enter_surf_penetration_mod < 0.1f))
		data.penetrate_count = 0;

	if (data.penetrate_count <= 0)
		return false;

	Vector dummy;

	trace_t trace_exit;

	if (!TraceToExit(dummy, &data.enter_trace, data.enter_trace.endpos, data.direction, &trace_exit))
		return false;

	surfacedata_t *exit_surface_data =g_pPhysics->GetSurfaceData(trace_exit.surface.surfaceProps);

	int exit_material = exit_surface_data->game.material;

	float exit_surf_penetration_mod = exit_surface_data->game.flPenetrationModifier;
	float final_damage_modifier = 0.16f;
	float combined_penetration_modifier = 0.0f;

	if (((data.enter_trace.contents & CONTENTS_GRATE) != 0) || (enter_material == 89) || (enter_material == 71))
	{
		combined_penetration_modifier = 3.0f;
		final_damage_modifier = 0.05f;
	}
	else
	{
		combined_penetration_modifier = (enter_surf_penetration_mod + exit_surf_penetration_mod) * 0.5f;
	}

	if (enter_material == exit_material)
	{
		if (exit_material == 87 || exit_material == 85)
			combined_penetration_modifier = 3.0f;
		else if (exit_material == 76)
			combined_penetration_modifier = 2.0f;
	}

	
	auto flModifier = fmaxf(0.f, 1.0f / combined_penetration_modifier);
	auto flTakenDamage = (data.current_damage * final_damage_modifier) + flModifier * 3.f * fmaxf(0.f, (3.f / wpn_data->flPenetration) * 1.25f);
	auto flThickness = VectorLength(trace_exit.endpos - data.enter_trace.endpos);

	flThickness *= flThickness;
	flThickness *= flModifier;
	flThickness /= 24.f;


	auto flTravelDamage = fmaxf(0.f, flTakenDamage + flThickness);

	if (flTravelDamage > data.current_damage)
		return false;

	if (flTravelDamage >= 0.0f)
		data.current_damage -= flTravelDamage;

	if (data.current_damage < 1.0f)
		return false;

	data.src = trace_exit.endpos;
	data.penetrate_count--;
	


	return true;
}
bool CAutowall::SimulateFireBullet(CBaseEntity* pBaseEntity, FireBulletData& data)
{
	//Utils::ToLog("SimulateFireBullet");
	data.penetrate_count = 4;
	data.trace_length = 0.0f;
	auto *wpn_data = G::MainWeapon->GetCSWpnData();

	data.current_damage = (float)wpn_data->iDamage;

	while ((data.penetrate_count > 0) && (data.current_damage >= 1.0f))
	{
		data.trace_length_remaining = wpn_data->flRange - data.trace_length;

		Vector end = data.src + data.direction * data.trace_length_remaining;

		GameUtils::TraceLine(data.src, end, 0x4600400B, G::LocalPlayer, &data.enter_trace);

		//proper way but seems to have a bad performance?
		//UTIL_ClipTraceToPlayers(data.src, end + data.direction * 40.f, 0x4600400B, &data.filter, &data.enter_trace);
		

		if (data.enter_trace.fraction == 1.0f)
			break;
		ClientClass* pClass = (ClientClass*)data.enter_trace.m_pEnt->GetClientClass();
		if ((data.enter_trace.hitgroup <= 7) && (data.enter_trace.hitgroup > 0) && (data.enter_trace.m_pEnt->GetTeamNum() != G::LocalPlayer->GetTeamNum()) && pClass->m_ClassID != (int)CSGOClassID::CHostage)
		{
			data.trace_length += data.enter_trace.fraction * data.trace_length_remaining;
			data.current_damage *= (float)pow(wpn_data->flRangeModifier, data.trace_length * 0.002);

			ScaleDamage(data.enter_trace.hitgroup, data.enter_trace.m_pEnt, wpn_data->flArmorRatio, data.current_damage);

			return true;
		}

		if (/*!RagebotElements.Ragebot_Autowall->Checked || */!HandleBulletPenetration(wpn_data, data))
			break;
	}

	return false;
}
bool CAutowall::PenetrateWall(CBaseEntity* pBaseEntity, const Vector& vecPoint)
{
	auto AngleVectors = [](const Vector &angles, Vector *forward)
	{
		Assert(s_bMathlibInitialized);
		Assert(forward);

		float	sp, sy, cp, cy;

		sy = sin(DEG2RAD(angles[1]));
		cy = cos(DEG2RAD(angles[1]));

		sp = sin(DEG2RAD(angles[0]));
		cp = cos(DEG2RAD(angles[0]));

		forward->x = cp*cy;
		forward->y = cp*sy;
		forward->z = -sp;
	};

	auto data = FireBulletData(G::LocalPlayer->GetEyePosition());
	data.filter = CTraceFilter();
	data.filter.pSkip = G::LocalPlayer;

	Vector angles = GameUtils::CalculateAngle(data.src, vecPoint);
	AngleVectors(angles, &data.direction);
	VectorNormalize(data.direction);

	if (SimulateFireBullet(G::LocalPlayer, data))
	{


		float min_damage = RagebotElements.Ragebot_MinDamage->value;
		if (pBaseEntity->GetHealth() < min_damage)
			min_damage = pBaseEntity->GetHealth();

		if(data.current_damage > min_damage)
			return true;
	}

	return false;
}

