#include "hooks.h"
#include "GameUtils.h"
#include "global.h"
#include "BacktrackingHelper.h"
#include "Menu.h"
#include "Autowall.h"
bool CGameTrace::DidHitWorld() const
{
	return m_pEnt == g_pEntitylist->GetClientEntity(0);
}
bool CGameTrace::DidHit() const
{
	return fraction < 1.0f || allsolid || startsolid;
}

// Returns true if we hit something and it wasn't the world.
bool CGameTrace::DidHitNonWorldCBaseEntity() const
{
	return m_pEnt != NULL && !DidHitWorld();
}

bool GameUtils::WorldToScreen(const Vector& in, Vector& position)
{
	return (g_pDebugOverlay->ScreenPosition(in, position) != 1);
}
std::vector<Vector> GameUtils::GetMultiplePointsForHitbox(CBaseEntity* pBaseEntity, int iHitbox, VMatrix BoneMatrix[128])
{
	std::vector<Vector> vPoints;

	auto VectorTransform_Wrapper = [](const Vector& in1, const VMatrix &in2, Vector &out)
	{
		auto VectorTransform = [](const float *in1, const VMatrix& in2, float *out)
		{
			auto DotProducts = [](const float *v1, const float *v2)
			{
				return v1[0] * v2[0] + v1[1] * v2[1] + v1[2] * v2[2];
			};
			out[0] = DotProducts(in1, in2[0]) + in2[0][3];
			out[1] = DotProducts(in1, in2[1]) + in2[1][3];
			out[2] = DotProducts(in1, in2[2]) + in2[2][3];
		};
		VectorTransform(&in1.x, in2, &out.x);
	};
	if (!pBaseEntity)
		return vPoints;

	

	studiohdr_t* pStudioModel = g_pModelInfo->GetStudioModel(pBaseEntity->GetModel());
	mstudiohitboxset_t* set = pStudioModel->pHitboxSet(0);

	if (!set)
		return vPoints;

	mstudiobbox_t* untransformedBox = set->pHitbox(iHitbox);
	if (!untransformedBox)
		return vPoints;



	Vector vecMin;
	VectorTransform_Wrapper(untransformedBox->bbmin, BoneMatrix[untransformedBox->bone], vecMin);

	Vector vecMax;
	VectorTransform_Wrapper(untransformedBox->bbmax, BoneMatrix[untransformedBox->bone], vecMax);
	

	Vector center = (vecMin + vecMax) * 0.5f;


	int type = RagebotElements.Ragebot_Corrections_HeightAdjustment->SelectedIndex;
	if (iHitbox == 0 && pBaseEntity->GetEyeAngles().x >= 66 && type > 0)
	{
			trace_t tr;
			Ray_t ray;
			CTraceCBaseEntity filter;

			filter.pHit = pBaseEntity;
			ray.Init(center + Vector(0, 0, 15), center);
			g_pEngineTrace->TraceRay_NEW(ray, MASK_SHOT, &filter, &tr);

			if (type == 1)
			{
				center.z += ((5.3f / 89) * pBaseEntity->GetEyeAngles().x);
			}
			else
			{
				if ((tr.endpos.z - center.z) > 1  && (tr.endpos.z - center.z) < 6 && (tr.fraction == 1.f || tr.m_pEnt == pBaseEntity))
				{
					center = tr.endpos;
					center.z -= 0.5;//0.7
				}
				else
				{
					center.z += (5.3 / 89) * pBaseEntity->GetEyeAngles().x;
				}
			}
	}
	vPoints.push_back(center);
	if (RagebotElements.Ragebot_Multipoint->SelectedIndex == 0)
		return vPoints;

	//try without multipoint ty
	//return vPoints;

	float rad = (untransformedBox->radius / 2) * RagebotElements.Ragebot_Multipoint_Scale->value;

	Vector top = Vector(center.x, center.y, center.z + rad);
	vPoints.push_back(Vector(top.x + rad, top.y + rad, top.z));
	vPoints.push_back(Vector(top.x + rad, top.y - rad, top.z));
	vPoints.push_back(Vector(top.x - rad, top.y + rad, top.z));
	vPoints.push_back(Vector(top.x - rad, top.y - rad, top.z));
	Vector bot = Vector(center.x, center.y, center.z - rad);
	vPoints.push_back(Vector(bot.x + rad, bot.y + rad, bot.z));
	vPoints.push_back(Vector(bot.x + rad, bot.y - rad, bot.z));
	vPoints.push_back(Vector(bot.x - rad, bot.y + rad, bot.z));
	vPoints.push_back(Vector(bot.x - rad, bot.y - rad, bot.z));
	if (RagebotElements.Ragebot_Multipoint->SelectedIndex == 2)
	{
		vPoints.push_back(Vector(center.x, center.y, center.z + rad));
		vPoints.push_back(Vector(center.x + rad, center.y, center.z));
		vPoints.push_back(Vector(center.x - rad, center.y, center.z));
		vPoints.push_back(Vector(center.x, center.y + rad, center.z));
		vPoints.push_back(Vector(center.x, center.y - rad, center.z));
	}
	return vPoints;
}
Vector GameUtils::GetHitboxVector(CBaseEntity* pBaseEntity, int iHitbox, matrix3x4 BoneMatrixArray[128])
{
	

	auto VectorTransform_Wrapper = [](const Vector& in1, const VMatrix &in2, Vector &out)
	{
		auto VectorTransform = [](const float *in1, const VMatrix& in2, float *out)
		{
			auto DotProducts = [](const float *v1, const float *v2)
			{
				return v1[0] * v2[0] + v1[1] * v2[1] + v1[2] * v2[2];
			};
			out[0] = DotProducts(in1, in2[0]) + in2[0][3];
			out[1] = DotProducts(in1, in2[1]) + in2[1][3];
			out[2] = DotProducts(in1, in2[2]) + in2[2][3];
		};
		VectorTransform(&in1.x, in2, &out.x);
	};

	Vector  hitbox;

	studiohdr_t* pStudioModel = g_pModelInfo->GetStudioModel(pBaseEntity->GetModel());
	mstudiohitboxset_t* set = pStudioModel->pHitboxSet(0);

	if (!set)
		return hitbox;

	mstudiobbox_t* untransformedBox = set->pHitbox(iHitbox);
	if (!untransformedBox)
		return hitbox;



	Vector vecMin;
	VectorTransform_Wrapper(untransformedBox->bbmin, BoneMatrixArray[untransformedBox->bone], vecMin);

	Vector vecMax;
	VectorTransform_Wrapper(untransformedBox->bbmax, BoneMatrixArray[untransformedBox->bone], vecMax);

	hitbox = (vecMin + vecMax) * 0.5f;

	
	return hitbox;
}
Vector GameUtils::GetBonePosition(CBaseEntity* pPlayer,  int Bone, matrix3x4 MatrixArray[128])
{
	Vector pos = Vector();

	matrix3x4 & HitboxMatrix = MatrixArray[Bone];

	pos = Vector(HitboxMatrix[0][3], HitboxMatrix[1][3], HitboxMatrix[2][3]);

	return pos;
}
void GameUtils::TraceLine(Vector& vecAbsStart, Vector& vecAbsEnd, unsigned int mask, CBaseEntity* ignore, trace_t* ptr)
{
	Ray_t ray;
	ray.Init(vecAbsStart, vecAbsEnd);
	CTraceFilter filter;
	filter.pSkip = ignore;

	g_pEngineTrace->TraceRay(ray, mask, &filter, ptr);
}

bool GameUtils::IsVisible_Fix(Vector vecOrigin, Vector vecOther, unsigned int mask, CBaseEntity* pCBaseEntity, CBaseEntity* pIgnore, int& hitgroup)
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

	Ray_t ray;
	trace_t tr;
	ray.Init(vecOrigin, vecOther);
	CTraceFilter filter;
	filter.pSkip = pIgnore;

	g_pEngineTrace->TraceRay(ray, mask, &filter, &tr);

	//not correctly done tho

	//Vector direction;

	//Vector angles = GameUtils::CalculateAngle(vecOrigin, vecOther);
	//AngleVectors(angles, &direction);
	//VectorNormalize(direction);

	//UTIL_ClipTraceToPlayers(vecOrigin, vecOrigin + direction*40.f, 0x4600400B, &filter, &tr);

	hitgroup = tr.hitgroup;

	return (tr.m_pEnt == pCBaseEntity || tr.fraction >= 1.0f);
}
float GameUtils::GetFoV(QAngle qAngles, Vector vecSource, Vector vecDestination, bool bDistanceBased)
{
	auto MakeVector = [](QAngle qAngles)
	{
		auto ret = Vector();
		auto pitch = float(qAngles[0] * M_PI / 180.f);
		auto yaw = float(qAngles[1] * M_PI / 180.f);
		auto tmp = float(cos(pitch));
		ret.x = float(-tmp * -cos(yaw));
		ret.y= float(sin(yaw)*tmp);
		ret.z= float(-sin(pitch));
		return ret;
	};

	Vector ang, aim;
	double fov;

	ang = CalculateAngle(vecSource, vecDestination);
	aim = MakeVector(qAngles);
	ang = MakeVector(ang);

	auto mag_s = sqrt((aim[0] * aim[0]) + (aim[1] * aim[1]) + (aim[2] * aim[2]));
	auto mag_d = sqrt((aim[0] * aim[0]) + (aim[1] * aim[1]) + (aim[2] * aim[2]));
	auto u_dot_v = aim[0] * ang[0] + aim[1] * ang[1] + aim[2] * ang[2];

	fov = acos(u_dot_v / (mag_s*mag_d)) * (180.f / M_PI);

	if (bDistanceBased)
	{
		fov *= 1.4;
		float xDist = abs(vecSource[0] - vecDestination[0]);
		float yDist = abs(vecSource[1] - vecDestination[1]);
		float Distance = sqrt((xDist * xDist) + (yDist * yDist));

		Distance /= 650.f;

		if (Distance < 0.7f)
			Distance = 0.7f;

		if (Distance > 6.5)
			Distance = 6.5;

		fov *= Distance;
	}

	return (float)fov;
}
QAngle GameUtils::CalculateAngle(Vector vecOrigin, Vector vecOther)
{
	auto ret = Vector();
	Vector delta = vecOrigin - vecOther;
	double hyp = delta.Length2D();
	ret.y = (atan(delta.y / delta.x) * 57.295779513082f);
	ret.x = (atan(delta.z / hyp) * 57.295779513082f);
	ret[2] = 0.00;

	if (delta.x >= 0.0)
		ret.y += 180.0f;
	return ret;
}
bool GameUtils::IsAbleToShoot()
{
	if (!g_pGlobals)
		return false;

	if (!G::LocalPlayer || !G::MainWeapon)
		return false;

	auto flServerTime = (float)G::LocalPlayer->TickBase() * g_pGlobals->interval_per_tick;
	auto flNextPrimaryAttack = G::MainWeapon->NextPrimaryAttack();

	return(!(flNextPrimaryAttack > flServerTime));
}
bool GameUtils::IsBreakableEntity(CBaseEntity* pBaseEntity)
{
	if (!pBaseEntity)
		return false;

	if (pBaseEntity->GetCollisionGroup() != COLLISION_GROUP_PUSHAWAY && pBaseEntity->GetCollisionGroup() != COLLISION_GROUP_BREAKABLE_GLASS && pBaseEntity->GetCollisionGroup() != COLLISION_GROUP_NONE)
		return false;

	if (pBaseEntity->GetHealth() > 200)
		return false;

	IMultiplayerPhysics* pPhysicsInterface = (IMultiplayerPhysics*)pBaseEntity;
	if (pPhysicsInterface)
	{
		if (pPhysicsInterface->GetMultiplayerPhysicsMode() != PHYSICS_MULTIPLAYER_SOLID)
			return false;
	}
	else
	{
		ClientClass * pClass = (ClientClass*)pBaseEntity->GetClientClass();
		if (!string(pClass->m_pNetworkName).compare(XorStr("func_breakable")) || !string(pClass->m_pNetworkName).compare(XorStr("func_breakable_surf")))
		{
			if (!string(pClass->m_pNetworkName).compare(XorStr("func_breakable_surf")))
				if (pBaseEntity->IsBroken())
					return false;
		}
		else if (pBaseEntity->PhysicsSolidMaskForEntity() & CONTENTS_PLAYERCLIP)
			return false;
	}

	IBreakableWithPropData* pBreakableInterface = (IBreakableWithPropData*)pBaseEntity;
	if (pBreakableInterface)
		if (pBreakableInterface->GetDmgModBullet() <= 0.0f)
			return false;

	return true;
}