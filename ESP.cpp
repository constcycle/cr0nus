#include "sdk.h"
#include "ESP.h"
#include "global.h"
#include "Render.h"
#include "xor.h"
#include "Math.h"
#include "Menu.h"
#include "Misc.h"
#include "NoSpread.h"
#include "BacktrackingHelper.h"
#include "GameUtils.h"
#include "Resolver.h"
#include "Draw.h"
CESP* g_ESP = new CESP;
#define TICK_INTERVAL			( g_pGlobals->interval_per_tick )
#define TIME_TO_TICKS( dt )		( (int)( 0.5f + (float)(dt) / TICK_INTERVAL ) )
#define TICKS_TO_TIME( t )		( TICK_INTERVAL *( t ) )

static char weaponNames[55][32] = {
	"NONE",
	"DEAGLE",
	"DUALS",
	"FIVE-SEVEN",
	"GLOCK",
	"UNKNOWN",
	"UNKNOWN",
	"AK-47",
	"AUG",
	"AWP",
	"FAMAS",
	"G3SG1",
	"UNKNOWN",
	"GALIL",
	"M249",
	"UNKNOWN",
	"M4A4",
	"MAC-10",
	"UNKNOWN",
	"P90",
	"UNKNOWN",
	"UNKNOWN",
	"UNKNOWN",
	"UNKNOWN",
	"UMP-45",
	"XM1014",
	"BIZON",
	"MAG-7",
	"NEGEV",
	"SAWED-OFF",
	"TEC-9",
	"ZEUS",
	"P2000",
	"MP7",
	"MP9",
	"NOVA",
	"P250",
	"UNKNOWN",
	"SCAR-20",
	"SG-553",
	"SSG-08",
	"UNKNOWN",
	"KNIFE",
	"FLASHBANG",
	"GRENADE",
	"SMOKE",
	"MOLOTOV",
	"DECOY",
	"INCENDIARY",
	"BOMB",
	"M4A1-S",
	"USP-S",
	"CZ-75",
	"REVOLVER",
	"KNIFE"
};
char* WeaponIDName(int weaponID) {
	char* name = 0;
	if (weaponID == 59) //terrorist knife
		name = weaponNames[54];
	if (weaponID == 60) //M4A1-S
		name = weaponNames[50];
	if (weaponID == 61) //USP-S
		name = weaponNames[51];
	if (weaponID == 63) //CZ-75A
		name = weaponNames[52];
	if (weaponID == 64)//revolver
		name = weaponNames[53];
	if (weaponID > 64) //non-faction-specific knife
		name = "KNIFE";// weaponNames[54];
	if (!name) //none of the above
		name = weaponNames[weaponID];
	return name;
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
bool GetCBaseEntityBox(CBaseEntity* pBaseEntity, Vector& BotCenter, Vector& TopCenter, float& Left, float& Right, bool is_player)
{
	if (!pBaseEntity)
		return false;
	if (!is_player)
	{
		const VMatrix& trans = pBaseEntity->GetCollisionBoundTrans();

		CollisionProperty* collision = pBaseEntity->GetCollision();
		if (!collision)
			return false;

		Vector min, max;
		//if (!pBaseEntity->GetRenderable())
			//return false;
		//pBaseEntity->GetRenderable()->GetRenderBounds(min, max);
		min = collision->VecMins();
		max = collision->VecMaxs();



		Vector points[] = { Vector(min.x, min.y, min.z),
			Vector(min.x, max.y, min.z),
			Vector(max.x, max.y, min.z),
			Vector(max.x, min.y, min.z),
			Vector(max.x, max.y, max.z),
			Vector(min.x, max.y, max.z),
			Vector(min.x, min.y, max.z),
			Vector(max.x, min.y, max.z) };




		auto vector_transform = [](const Vector in1, const VMatrix& in2)
		{
			auto dot_product = [](const Vector &v1, const float *v2)
			{
				float ret = v1[0] * v2[0] + v1[1] * v2[1] + v1[2] * v2[2];
				return ret;
			};

			auto out = Vector();
			out[0] = dot_product(in1, in2[0]) + in2[0][3];
			out[1] = dot_product(in1, in2[1]) + in2[1][3];
			out[2] = dot_product(in1, in2[2]) + in2[2][3];
			return out;
		};

		Vector pointsTransformed[8];
		for (int i = 0; i < 8; i++)
		{
			pointsTransformed[i] = vector_transform(points[i], trans);
		}

		Vector pos = pBaseEntity->GetOrigin();
		Vector flb;
		Vector brt;
		Vector blb;
		Vector frt;
		Vector frb;
		Vector brb;
		Vector blt;
		Vector flt;


		if (!GameUtils::WorldToScreen(pointsTransformed[3], flb) || !GameUtils::WorldToScreen(pointsTransformed[5], brt)
			|| !GameUtils::WorldToScreen(pointsTransformed[0], blb) || !GameUtils::WorldToScreen(pointsTransformed[4], frt)
			|| !GameUtils::WorldToScreen(pointsTransformed[2], frb) || !GameUtils::WorldToScreen(pointsTransformed[1], brb)
			|| !GameUtils::WorldToScreen(pointsTransformed[6], blt) || !GameUtils::WorldToScreen(pointsTransformed[7], flt))
			return false;

		Vector arr[] = { flb, brt, blb, frt, frb, brb, blt, flt };
		//+1 for each cuz of borders at the original box
		float left = flb.x;        // left
		float top = flb.y;        // top
		float right = flb.x;    // right
		float bottom = flb.y;    // bottom

		for (int i = 1; i < 8; i++)
		{
			if (left > arr[i].x)
				left = arr[i].x;
			if (top < arr[i].y)
				top = arr[i].y;
			if (right < arr[i].x)
				right = arr[i].x;
			if (bottom > arr[i].y)
				bottom = arr[i].y;
		}
		BotCenter = Vector(right - ((right - left) / 2), top, 0);
		TopCenter = Vector(right - ((right - left) / 2), bottom, 0);
		Left = left;
		Right = right;
	}
	else
	{
		Vector org = pBaseEntity->GetOrigin();
		Vector head;
		if ((pBaseEntity->GetFlags() & FL_DUCKING))
			head = org + Vector(0.f, 0.f, 52.f);
		else
			head = org + Vector(0.f, 0.f, 72.f);

		Vector org_screen, head_screen;
		if (!GameUtils::WorldToScreen(org, org_screen) || !GameUtils::WorldToScreen(head, head_screen))
			return false;

		int height = int(org_screen.y - head_screen.y);
		int width = int(height / 2);
		Left = int(head_screen.x - width / 2);
		Right = int(head_screen.x + width / 2);
		BotCenter = Vector(head_screen.x, org_screen.y);
		TopCenter = BotCenter; TopCenter.y = head_screen.y;
	}
	return true;
}
void CESP::PredictNade(CBaseEntity* pPlayer, QAngle vViewAngles )
{
    if( !pPlayer )
        return;

	auto Draw3DBox = [](Vector Org, Vector vMin, Vector vMax, int r, int g, int b, int a)
	{
		Vector _min = Org + vMin;
		Vector _max = Org + vMax;
		Vector min, max;

		Vector crnr2 = Vector(_max.x, _min.y, _min.z);
		Vector crnr3 = Vector(_max.x, _min.y, _max.z);
		Vector crnr4 = Vector(_min.x, _min.y, _max.z);
		Vector crnr5 = Vector(_min.x, _max.y, _max.z);
		Vector crnr6 = Vector(_min.x, _max.y, _min.z);
		Vector crnr7 = Vector(_max.x, _max.y, _min.z);
		Vector crnr2_;
		Vector crnr3_;
		Vector crnr4_;
		Vector crnr5_;
		Vector crnr6_;
		Vector crnr7_;

		if (GameUtils::WorldToScreen(_min, min) &&
			GameUtils::WorldToScreen(_max, max) &&
			GameUtils::WorldToScreen(crnr2, crnr2_) &&
			GameUtils::WorldToScreen(crnr3, crnr3_) &&
			GameUtils::WorldToScreen(crnr4, crnr4_) &&
			GameUtils::WorldToScreen(crnr5, crnr5_) &&
			GameUtils::WorldToScreen(crnr6, crnr6_) &&
			GameUtils::WorldToScreen(crnr7, crnr7_))
		{
			g_Draw.LineRGBA(min.x, min.y, crnr2_.x, crnr2_.y, r, g, b, a);
			g_Draw.LineRGBA(min.x, min.y, crnr4_.x, crnr4_.y, r, g, b, a);
			g_Draw.LineRGBA(min.x, min.y, crnr6_.x, crnr6_.y, r, g, b, a);
			g_Draw.LineRGBA(max.x, max.y, crnr5_.x, crnr5_.y, r, g, b, a);
			g_Draw.LineRGBA(max.x, max.y, crnr7_.x, crnr7_.y, r, g, b, a);
			g_Draw.LineRGBA(max.x, max.y, crnr3_.x, crnr3_.y, r, g, b, a);
			g_Draw.LineRGBA(crnr2_.x, crnr2_.y, crnr7_.x, crnr7_.y, r, g, b, a);
			g_Draw.LineRGBA(crnr2_.x, crnr2_.y, crnr3_.x, crnr3_.y, r, g, b, a);
			g_Draw.LineRGBA(crnr4_.x, crnr4_.y, crnr5_.x, crnr5_.y, r, g, b, a);
			g_Draw.LineRGBA(crnr4_.x, crnr4_.y, crnr3_.x, crnr3_.y, r, g, b, a);
			g_Draw.LineRGBA(crnr6_.x, crnr6_.y, crnr5_.x, crnr5_.y, r, g, b, a);
			g_Draw.LineRGBA(crnr6_.x, crnr6_.y, crnr7_.x, crnr7_.y, r, g, b, a);
		}
	};


	const float TIMEALIVE = 5.f;
	const float GRENADE_COEFFICIENT_OF_RESTITUTION = 0.4f;

	float fStep = 0.1f;
	float fGravity = 800.0f / 8.f;

	Vector vPos, vThrow, vThrow2;
	Vector vStart;

	int iCollisions = 0;



	vThrow[0] = vViewAngles[0];
	vThrow[1] = vViewAngles[1];
	vThrow[2] = vViewAngles[2];

	if (vThrow[0] < 0)
		vThrow[0] = -10 + vThrow[0] * ((90 - 10) / 90.0);
	else
		vThrow[0] = -10 + vThrow[0] * ((90 + 10) / 90.0);

	float fVel = (90 - vThrow[0]) * 4;
	if (fVel > 500)
		fVel = 500;

	Math::AngleVectors(vThrow, &vThrow2);

	Vector vEye = pPlayer->GetEyePosition();
	vStart[0] = vEye[0] + vThrow2[0] * 16;
	vStart[1] = vEye[1] + vThrow2[1] * 16;
	vStart[2] = vEye[2] + vThrow2[2] * 16;

	vThrow2[0] = (vThrow2[0] * fVel) + pPlayer->GetVelocity().x;
	vThrow2[1] = (vThrow2[1] * fVel) + pPlayer->GetVelocity().y;
	vThrow2[2] = (vThrow2[2] * fVel) + pPlayer->GetVelocity().z;	// casualhacker for da magic calc help

	for (float fTime = 0.0f; fTime < TIMEALIVE; fTime += fStep)
	{
		vPos = vStart + vThrow2 * fStep;

		Ray_t ray;
		trace_t tr;
		CTraceFilter loc;
		loc.pSkip = pPlayer;

		ray.Init(vStart, vPos);
		g_pEngineTrace->TraceRay(ray, MASK_SOLID, &loc, &tr);
		Vector vOutStart, vOutEnd;
		if (tr.DidHit())
		{
			vThrow2 = tr.plane.normal * -2.0f * DotProduct(vThrow2, tr.plane.normal) + vThrow2;
			vThrow2 *= GRENADE_COEFFICIENT_OF_RESTITUTION;

			iCollisions++;
			if (iCollisions > 2) // > 2
				break;

			vPos = vStart + vThrow2 * tr.fraction * fStep;
			fTime += (fStep * (1 - tr.fraction));
			if (g_pDebugOverlay->ScreenPosition(vPos, vOutEnd) != 1)
			{
				if (iCollisions < 2)
					Draw3DBox(vPos, Vector(-2, -2, -2), Vector(2, 2, 2), 0, 0, 255, 255);
				else
					Draw3DBox(vPos, Vector(-5, -5, -5), Vector(5, 5, 5), 255, 0, 0, 255); //<---- Boom here
			}
		}


		if (iCollisions <= 2)
		{
			if (g_pDebugOverlay->ScreenPosition(vStart, vOutStart) != 1, g_pDebugOverlay->ScreenPosition(vPos, vOutEnd) != 1)
			{
				g_Draw.LineRGBA(vOutStart.x, vOutStart.y, vOutEnd.x, vOutEnd.y, 255, 255, 255, 255);
			}
		}


		vStart = vPos;
		vThrow2.z -= fGravity * tr.fraction * fStep;
	}
}
void CESP::DrawBones(CBaseEntity* pBaseEntity, int r, int g, int b, int a)
{
	int index = pBaseEntity->Index();
	studiohdr_t* pStudioModel = g_pModelInfo->GetStudioModel((model_t*)pBaseEntity->GetModel());
	if (pStudioModel && g_BacktrackHelper->PlayerRecord[index].records.size() > 0)
	{
		//for (int j = 0; j < g_BacktrackHelper->PlayerRecord[index].records.size(); j++)
		if(g_BacktrackHelper->PlayerRecord[index].records.size() > 0)
		{
			tick_record record = g_BacktrackHelper->PlayerRecord[index].records.back();



			float lerptime = g_BacktrackHelper->GetLerpTime();
			float desired_time = record.m_flSimulationTime + lerptime;
			float estimated_time = g_BacktrackHelper->GetEstimateServerTime();



			float SV_MAXUNLAG = 1.0f;


			float latency = g_BacktrackHelper->GetNetworkLatency();
			float m_flLerpTime = g_BacktrackHelper->GetLerpTime();
			float correct = clamp<float>(latency + m_flLerpTime, 0.0f, SV_MAXUNLAG);

			float deltaTime = correct - (estimated_time + lerptime - desired_time);

			if (fabs(deltaTime) <= 0.2f)
			{
		
				for (int i = 0; i < pStudioModel->numbones; i++)
				{
					mstudiobone_t* pBone = pStudioModel->pBone(i);
					if (!pBone || !(pBone->flags & 256) || pBone->parent == -1)
						continue;

					Vector sBonePos1 = GameUtils::GetBonePosition(pBaseEntity, i, record.boneMatrix), vBonePos1; //.back() for nice skeleton
					if (sBonePos1 == Vector(0, 0, 0))
						continue;
					if (!GameUtils::WorldToScreen(sBonePos1, vBonePos1))
						continue;

					Vector sBonePos2 = GameUtils::GetBonePosition(pBaseEntity, pBone->parent, record.boneMatrix), vBonePos2;//.back() for nice skeleton
					if (sBonePos2 == Vector(0, 0, 0))
						continue;
					if (!GameUtils::WorldToScreen(sBonePos2, vBonePos2))
						continue;

					//g_pRender->Line((int)vBonePos1.x, (int)vBonePos1.y, (int)vBonePos2.x, (int)vBonePos2.y, D3DCOLOR_RGBA(r, g, b, a));
					Color col = record.bLowerBodyYawUpdated ? Color(0, 255, 0, 255) : Color(255, 255, 255, 255);
					if (record.needs_extrapolation)
						col = Color(255, 40, 0, 255);

					g_Draw.LineRGBA((int)vBonePos1.x, (int)vBonePos1.y, (int)vBonePos2.x, (int)vBonePos2.y, col.r(), col.g(), col.b(), 255);

					if (record.needs_extrapolation)
					{
						Vector position = record.m_vecOrigin;
						Vector extr_position = position;
						float simtime = record.m_flSimulationTime;
						g_BacktrackHelper->ExtrapolatePosition(pBaseEntity, extr_position, simtime, record.m_vecVelocity);

						Vector sBonePos1 = GameUtils::GetBonePosition(pBaseEntity, i, record.boneMatrix), vBonePos1; //.back() for nice skeleton

						

						if (sBonePos1 == Vector(0, 0, 0))
							continue;

						sBonePos1 -= position;
						sBonePos1 += extr_position;

						if (!GameUtils::WorldToScreen(sBonePos1, vBonePos1))
							continue;

						Vector sBonePos2 = GameUtils::GetBonePosition(pBaseEntity, pBone->parent, record.boneMatrix), vBonePos2;//.back() for nice skeleton

						

						if (sBonePos2 == Vector(0, 0, 0))
							continue;

						sBonePos2 -= position;
						sBonePos2 += extr_position;

						if (!GameUtils::WorldToScreen(sBonePos2, vBonePos2))
							continue;

						Color col = Color(0, 0, 255);

						g_Draw.LineRGBA((int)vBonePos1.x, (int)vBonePos1.y, (int)vBonePos2.x, (int)vBonePos2.y, col.r(), col.g(), col.b(), 255);
					}
				}

				
			}
		}
	}
	
}
void CESP::DrawPlayer(CBaseEntity* pPlayer, CBaseEntity* pLocalPlayer)
{
	Vector m_position = pPlayer->GetOrigin();
	Vector m_top_position = m_position;
	if (pPlayer->GetFlags() & IN_DUCK)
		m_top_position += Vector(0, 0, 52);
	else
		m_top_position += Vector(0, 0, 72);

	Vector footpos, headpos;

	

	if (GameUtils::WorldToScreen(m_position, footpos) && GameUtils::WorldToScreen(m_top_position, headpos))
	{
		/*int height = footpos.y - headpos.y;
		int width = int((height / 4.5f) * 2);
		int x = headpos.x - width / 2;
		int y = headpos.y;*/
		Vector Bot, Top;
		float Left, Right;

		GetCBaseEntityBox(pPlayer, Bot, Top, Left, Right, true);

		int height = Bot.y - Top.y;
		int width = Right - Left;
		int x = Left;
		int y = Top.y;

		bool isEnemyVis = g_pEngineTrace->IsVisible(pLocalPlayer, pLocalPlayer->GetEyePosition(), pPlayer->GetEyePosition(), pPlayer);

		

		DWORD col;
		if (pPlayer->GetTeamNum() == 2/*terrorists*/)
			col = isEnemyVis ? ColorElements.Color_T_Visible->color : ColorElements.Color_T_Invisible->color;
		else if (pPlayer->GetTeamNum() == 3/*pBaseEntity->GetTeamNum() == pLocalCBaseEntity->GetTeamNum()*/)
			col = isEnemyVis ? ColorElements.Color_CT_Visible->color : ColorElements.Color_CT_Invisible->color;


		CBaseCombatWeapon* pMainWeapon = pPlayer->GetWeapon();
		if (VisualElements.Visual_Player_Bones->Checked)
		{
			DrawBones(pPlayer, 255, 255, 255, 255);//col.r(), col.g(), col.b(), 255);
		}
		if (VisualElements.Visual_Player_Snaplines->Checked && pPlayer != pLocalPlayer)
		{
			g_Draw.LineRGBA((G::Screen.width / 2) + 1, G::Screen.height, x+width/2 + 1, y+height, 0, 0, 0, 255);
			g_Draw.LineRGBA(G::Screen.width / 2, G::Screen.height, x + width / 2, y + height, getR(col), getG(col), getB(col), 255);
			g_Draw.LineRGBA((G::Screen.width / 2) - 1, G::Screen.height, x + width / 2 - 1, y + height, 0, 0, 0, 255);
		}
		if (VisualElements.Visual_Player_Grenade_Prediction->Checked && pPlayer != pLocalPlayer)
		{
			if (pMainWeapon && pMainWeapon->IsGrenade())
			{
				PredictNade(pPlayer, pPlayer->GetEyeAngles());
			}
		}
		if (pPlayer == pLocalPlayer)
		{
			/*show lby and shit*/
			Vector vecLby, vecReal, vecFake, vecCircle;
			Vector org = G::LocalPlayer->GetOrigin(), vOrg;
			Vector end, vEnd;
			Math::AngleVectors(Vector(0, Math::NormalizeYaw(G::LocalPlayer->LowerBodyYaw()), 0), &vecLby);
			Math::AngleVectors(Vector(0, Math::NormalizeYaw(G::RealAngle.y), 0), &vecReal);
			Math::AngleVectors(Vector(0, Math::NormalizeYaw(G::FakeAngle.y), 0), &vecFake);
			Vector circle_angle = Vector(0, Math::NormalizeYaw(g_Misc->m_circle_yaw), 0);
			vecCircle = circle_angle.Direction();
			
			
			end = org + (vecFake * 70);

			if (GameUtils::WorldToScreen(org, vOrg) && GameUtils::WorldToScreen(end, vEnd))
			{
				g_Draw.LineRGBA(vOrg.x, vOrg.y, vEnd.x, vEnd.y, 0, 255, 0, 255);
			}
			end = org + (vecReal * 70);

			if (GameUtils::WorldToScreen(org, vOrg) && GameUtils::WorldToScreen(end, vEnd))
			{
				g_Draw.LineRGBA(vOrg.x, vOrg.y, vEnd.x, vEnd.y, 255, 0, 0, 255);
			}
			end = org + (vecLby * 70);

			if (GameUtils::WorldToScreen(org, vOrg) && GameUtils::WorldToScreen(end, vEnd))
			{
				g_Draw.LineRGBA(vOrg.x, vOrg.y, vEnd.x, vEnd.y, 0, 0, 255, 255);
			}
			Vector vel = G::LocalPlayer->GetVelocity().Normalized();

			end = org + (vecCircle * (vel.Length2D()));

			if (GameUtils::WorldToScreen(org, vOrg) && GameUtils::WorldToScreen(end, vEnd))
			{
				g_Draw.LineRGBA(vOrg.x, vOrg.y, vEnd.x, vEnd.y, 255, 0, 255, 255);
			}
		}
		if (VisualElements.Visual_Player_DLight->Checked)
		{
			dlight_t* pDlight = g_pEffects->CL_AllocDlight(pPlayer->Index());

			pDlight->origin = pPlayer->GetOrigin();
			pDlight->radius = 75;
			pDlight->color = Color(getR(col), getG(col), getB(col)); pDlight->color.SetAlpha(10);
			pDlight->die = g_pGlobals->curtime + 0.05f;
			pDlight->decay = pDlight->radius / 5.0f;
			pDlight->key = pPlayer->Index();


			dlight_t* pElight = g_pEffects->CL_AllocElight(pPlayer->Index());
			pElight->origin = pPlayer->GetOrigin() + Vector(0.0f, 0.0f, 35.0f);
			pElight->radius = 75;
			pElight->color = Color(getR(col), getG(col), getB(col)); pElight->color.SetAlpha(10);
			pElight->die = g_pGlobals->curtime + 0.05f;
			pElight->decay = pElight->radius / 5.0f;
			pElight->key = pPlayer->Index();
		}



		if (VisualElements.Visual_Player_Box->Checked)
		{
			g_Draw.Box(x, y, width, height, 0, 0, 0, 255);
			g_Draw.Box(x + 1, y + 1, width - 2, height - 2, getR(col), getG(col), getB(col), 255);
			g_Draw.Box(x + 2, y + 2, width - 4, height - 4, 0, 0, 0, 255);
		}
		if (VisualElements.Visual_Player_Armor->Checked)
		{
			if (pPlayer->GetArmor() > 0)
			{
				int armor = 100-pPlayer->GetArmor();

				int w = 4;
				if (width < 4)
					w = width;

				int step_height = height / 4;

				//int distance = width / 8;


				g_Draw.FillRGBA(x + width, y + armor*height/100, w, pPlayer->GetArmor()*height/100, 18, 43, 255, 255);

				g_Draw.Box(x + width, y, w, height, 0, 0, 0, 255);

				for (int i = 1; i < 4; i++)
				{
					//if(i*25 >= pPlayer->GetArmor())
					g_Draw.LineRGBA(x + width, y + i*step_height, x + width + w-1, y + i*step_height, 0, 0, 0, 255);
				}
				
			}
		}
		if (VisualElements.Visual_Player_Health->Checked)
		{
			int health = 100-pPlayer->GetHealth();
			int w = 4;
			if (width < 4)
				w = width;

			int hr, hg, hb;


			hr = 255 - (pPlayer->GetHealth()*2.55);
			hg = pPlayer->GetHealth() * 2.55;
			hb = 0;

			//int distance = width / 8;

			g_Draw.FillRGBA(x - (w), y + health*height / 100, w, pPlayer->GetHealth()*height / 100, hr, hg, hb, 255);

			g_Draw.Box(x - (w), y, w , height, 0, 0, 0, 255);

			if (pPlayer->GetHealth() < 100)
			{
				char hp[50];
				sprintf_s(hp, sizeof(hp), "%i", pPlayer->GetHealth());

				g_Draw.StringA(g_Draw.font_esp, false, x - w - g_Draw.getWidht(hp, g_Draw.font_esp), y + health*height / 100 - 12, 200, 200, 200, 255, "%i", pPlayer->GetHealth());
			}
			int step_height = height / 4;
			for (int i = 1; i < 4; i++)
			{
				//if(i*25 >= pPlayer->GetArmor())
				g_Draw.LineRGBA(x - w, y + i*step_height, x, y + i*step_height, 0, 0, 0, 255);
			}
		}
		int bot_add = 0;
		int top_add = 0;
		if (VisualElements.Visual_Player_Name->Checked)
		{
			player_info_s info;
			g_pEngine->GetPlayerInfo(pPlayer->Index(), &info);
		
			g_Draw.StringA(g_Draw.font_esp, true, x + width / 2, y - 12 - top_add, 200, 200, 200, 255, "%s", info.m_szPlayerName);

			top_add += 12;


			

		}
		if (VisualElements.Visual_Player_Weapon->Checked)
		{
			UINT* hWeapons = pPlayer->GetWeapons();
			if (hWeapons)
			{

				if (pMainWeapon)
				{
					std::string s1 = WeaponIDName(pMainWeapon->WeaponID());
					if (VisualElements.Visual_Player_Weapon_Ammo->Checked && pMainWeapon->Clip1() != -1 && pMainWeapon->GetWeaponType() != WEAPONCLASS::WEPCLASS_INVALID)
					{
						s1.append(XorStr(" | "));
						s1.append(to_string(pMainWeapon->Clip1()));
					}
					if(VisualElements.Visual_Player_Weapon->Checked)
						g_Draw.StringA(g_Draw.font_esp, true, x + width / 2, y + height + bot_add, 200, 200, 200, 255, XorStr("%s"), s1.c_str());
					else
					{
						std::string ammo; ammo.append(" | "); ammo.append(to_string(pMainWeapon->Clip1()));
						int icon_width = g_Draw.getWidht(pMainWeapon->GetGunIcon(), g_Draw.font_weapon);
						int ammo_width = g_Draw.getWidht(ammo.c_str(), g_Draw.font_esp);
						g_Draw.StringA(g_Draw.font_weapon, false, x + width / 2 - (icon_width + ammo_width)/2, y + height + bot_add, 200, 200, 200, 255, XorStr("%s"), pMainWeapon->GetGunIcon());
						g_Draw.StringA(g_Draw.font_esp, false, x + width / 2 - (icon_width + ammo_width) / 2 + icon_width, y + height + bot_add, 200, 200, 200, 255, XorStr("%s"), ammo.c_str());
					}
					//g_pRender->String(x + width / 2, y + height + bot_add, centered, g_pRender->Fonts.weapon_icon, true, WHITE(255), "%s", pMainWeapon->GetGunIcon());


					bot_add += 12;
					if (VisualElements.Visual_Player_Weapon_All->Checked)
					{
						for (int nIndex = 0; hWeapons[nIndex]; nIndex++) //so we wanna start with 1 and do not use the primary
						{
							CBaseCombatWeapon* pWeapon = (CBaseCombatWeapon*)g_pEntitylist->GetClientEntityFromHandle(hWeapons[nIndex]);
							if (!pWeapon || pWeapon == pMainWeapon || pWeapon->GetWeaponType() == WEAPONCLASS::WEPCLASS_KNIFE)
								continue;

							std::string s = WeaponIDName(pWeapon->WeaponID());
							if (VisualElements.Visual_Player_Weapon_Ammo->Checked&& pWeapon->Clip1() != -1 && pWeapon->GetWeaponType() != WEAPONCLASS::WEPCLASS_INVALID)
							{
								s.append(XorStr(" | "));
								s.append(to_string(pWeapon->Clip1()));
							}
							g_Draw.StringA(g_Draw.font_esp, true, x + width / 2, y + height + bot_add, 200, 200, 200, 255, XorStr("%s"), s.c_str());
							
						
							//g_pRender->String(x + width / 2, y + height + bot_add ,centered, g_pRender->Fonts.weapon_icon, true, WHITE(255), "%s", pMainWeapon->GetGunIcon());

							bot_add += 12;
						}
					}
				}
			}
		}
		
	}
}
void CESP::DrawC4(CBaseEntity* pBomb, bool is_planted, CBaseEntity* pLocalPlayer)
{
	Vector Bot, Top;
	float Left, Right;
	GetCBaseEntityBox(pBomb, Bot, Top, Left, Right, false);
	int height = Bot.y - Top.y;
	int width = Right - Left;

	DWORD col = is_planted ? D3DCOLOR_RGBA(250, 42, 42, 255) : D3DCOLOR_RGBA(255, 255, 0, 255);


	if (VisualElements.Visual_CBaseEntity_Box->Checked)
	{
		g_Draw.Box(Left, Top.y, width, height, 0, 0, 0, 255);
		g_Draw.Box(Left + 1, Top.y + 1, width - 2, height - 2, getR(col), getG(col), getB(col), 255);
		g_Draw.Box(Left + 2, Top.y + 2, width - 4, height - 4, 0, 0, 0, 255);
	}
	if (VisualElements.Visual_CBaseEntity_Name->Checked)
	{

		//g_pRender->String(Left + width / 2, Bot.y, centered, g_pRender->Fonts.esp, true, WHITE(255), "%s", "Bomb");
		
		g_Draw.StringA(g_Draw.font_esp, true, Left + width / 2, Bot.y, 200, 200, 200, 255, "%s", "Bomb");
		
		if (is_planted)
		{
			CBomb* bomb = (CBomb*)pBomb;
			float flBlow = bomb->GetC4BlowTime();
			float TimeRemaining = flBlow - (g_pGlobals->interval_per_tick * pLocalPlayer->TickBase());
			if (TimeRemaining < 0)
				TimeRemaining = 0;
			char buffer[64];

			if (bomb->IsDefused())
				sprintf_s(buffer, XorStr("Defused"));
			else
				sprintf_s(buffer, XorStr("%.1fs remaining!"), TimeRemaining);


			//g_pRender->String(Left + width / 2, Bot.y + 13, centered, g_pRender->Fonts.esp, true, WHITE(255), "%s", buffer);
			g_Draw.StringA(g_Draw.font_esp, true, Left + width / 2, Bot.y + 13, 200, 200, 200, 255, "%s", buffer);

			if (!bomb->IsDefused())
			{
				float a = 450.7f;
				float b = 75.68f;
				float c = 789.2f;
				float d = ((pLocalPlayer->GetOrigin().DistTo(pBomb->GetOrigin()) - b) / c);
				float flDamage = a * expf(-d * d);

				auto GetArmourHealth = [](float flDamage, int ArmorValue)
				{
					float flCurDamage = flDamage;

					if (flCurDamage == 0.0f || ArmorValue == 0)
						return flCurDamage;

					float flArmorRatio = 0.5f;
					float flArmorBonus = 0.5f;
					float flNew = flCurDamage * flArmorRatio;
					float flArmor = (flCurDamage - flNew) * flArmorBonus;

					if (flArmor > ArmorValue)
					{
						flArmor = ArmorValue * (1.0f / flArmorBonus);
						flNew = flCurDamage - flArmor;
					}

					return flNew;
				};

				float damage = max((int)ceilf(GetArmourHealth(flDamage, pLocalPlayer->GetArmor())), 0);

				
				//g_pRender->String(Left + width / 2, Bot.y + 26, centered, g_pRender->Fonts.esp, true, WHITE(255), "Damage: %f", damage);
				g_Draw.StringA(g_Draw.font_esp, true, Left + width / 2, Bot.y + 26, 200, 200, 200, 255, "Damage: %f", damage);
			}
		}
	}
}
void CESP::DrawHostage(CBaseEntity* pHostage)
{
	Vector Bot, Top;
	float Left, Right;
	GetCBaseEntityBox(pHostage, Bot, Top, Left, Right, false);
	int height = Bot.y - Top.y;
	int width = Right - Left;

	DWORD col = ColorElements.Color_Hostage->color;

	if (VisualElements.Visual_CBaseEntity_Box->Checked)
	{
		//g_pRender->BorderedBox(Left, Top.y, width, height, BLACK(255));
		//g_pRender->BorderedBox(Left + 1, Top.y + 1, width - 2, height - 2, D3DCOLOR_RGBA(255, 255, 0, 255));
		//g_pRender->BorderedBox(Left + 2, Top.y + 2, width - 4, height - 4, BLACK(255));
		g_Draw.Box(Left, Top.y, width, height, 0, 0, 0, 255);
		g_Draw.Box(Left+1, Top.y+1, width-2, height-2, getR(col), getG(col), getB(col), 255);
		g_Draw.Box(Left+2, Top.y+2, width-4, height-4, 0, 0, 0, 255);
	}
	if (VisualElements.Visual_CBaseEntity_Name->Checked)
	{
		g_pRender->Text("Hostage", Left + width / 2, Bot.y, centered, g_pRender->Fonts.esp, true, WHITE(255), BLACK(255));
		g_Draw.StringA(g_Draw.font_esp, true, Left + width / 2 , Bot.y, 200, 200, 200, 255, "Hostage");
		
	}
	
}
void CESP::DrawThrowable(CBaseEntity* pThrowable)
{
	const model_t* nadeModel = pThrowable->GetModel();

	if (!nadeModel)
		return;

	studiohdr_t* hdr = g_pModelInfo->GetStudioModel(nadeModel);

	if (!hdr)
		return;

	if (!strstr(hdr->name, XorStr("thrown")) && !strstr(hdr->name, XorStr("dropped")))
		return;

	
	D3DCOLOR nadeColor = WHITE(255);

	std::string nadeName = XorStr("Unknown Grenade");

	IMaterial* mats[32];
	g_pModelInfo->GetModelMaterials(nadeModel, hdr->numtextures, mats);

	for (int i = 0; i < hdr->numtextures; i++)
	{
		IMaterial* mat = mats[i];
		if (!mat)
			continue;

		if (strstr(mat->GetName(), XorStr("flashbang")))
		{
			nadeName = XorStr("Flashbang");
			nadeColor = D3DCOLOR_RGBA(255, 255, 0, 255);
			break;
		}
		else if (strstr(mat->GetName(), XorStr("m67_grenade")) || strstr(mat->GetName(), XorStr("hegrenade")))
		{
			nadeName = XorStr("HE Grenade");
			nadeColor = D3DCOLOR_RGBA(250, 42, 42, 255);
			break;
		}
		else if (strstr(mat->GetName(), XorStr("smoke")))
		{
			nadeName = XorStr("Smoke");
			nadeColor = D3DCOLOR_RGBA(255, 255, 255, 255);
			break;
		}
		else if (strstr(mat->GetName(), XorStr("decoy")))
		{
			nadeName = XorStr("Decoy");
			nadeColor = D3DCOLOR_RGBA(0, 255, 0, 255);
			break;
		}
		else if (strstr(mat->GetName(), XorStr("incendiary")) || strstr(mat->GetName(), XorStr("molotov")))
		{
			nadeName = XorStr("Molotov");
			nadeColor = D3DCOLOR_RGBA(250, 42, 42, 255);
			break;
		}
	}
	Vector Bot, Top;
	float Left, Right;
	GetCBaseEntityBox(pThrowable, Bot, Top, Left, Right, false);
	int height = Bot.y - Top.y;
	int width = Right - Left;

	if (VisualElements.Visual_CBaseEntity_Box->Checked)
	{
		g_Draw.Box(Left, Top.y, width, height, 0, 0, 0, 255);
		g_Draw.Box(Left + 1, Top.y + 1, width - 2, height - 2, getR(nadeColor), getG(nadeColor), getB(nadeColor), 255);
		g_Draw.Box(Left + 2, Top.y + 2, width - 4, height - 4, 0, 0, 0, 255);
	}
	if (VisualElements.Visual_CBaseEntity_Name->Checked)
	{
		//g_pRender->Text((char*)nadeName.c_str(), Left + width / 2, Bot.y, centered, g_pRender->Fonts.esp, true, WHITE(255), BLACK(255));
		g_Draw.StringA(g_Draw.font_weapon, true, Left + width / 2, Bot.y, 255, 255, 255, 255, nadeName.c_str());
	}
}
void CESP::DrawDroppedWeapon(CBaseCombatWeapon* pWeapon)
{
	CBaseEntity* pCBaseEntity = (CBaseEntity*)pWeapon;
	CBaseCombatWeapon* Weapon = (CBaseCombatWeapon*)pWeapon;

	if (!pCBaseEntity || !g_pModelInfo)
		return;



	std::string sCurWeapon = g_pModelInfo->GetModelName(pCBaseEntity->GetModel());
	if (!sCurWeapon.empty() && sCurWeapon.find(XorStr("w_")) != std::string::npos)
	{
		std::string name;

		if (sCurWeapon.find(XorStr("defuser")) != std::string::npos/* insert check for defuser here*/)
			name = XorStr("defuser");
		else
		{
			CBaseCombatWeapon* pWeapon = (CBaseCombatWeapon*)pCBaseEntity;
			if (!pWeapon)return;
			name = WeaponIDName(pWeapon->WeaponID());//pWeapon->GetName(false);
		}
		Vector Bot, Top;
		float Left, Right;
		GetCBaseEntityBox(pCBaseEntity, Bot, Top, Left, Right, false);
		int height = Bot.y - Top.y;
		int width = Right - Left;

		if (VisualElements.Visual_CBaseEntity_Box->Checked)
		{
			//g_pRender->BorderedBox(Left, Top.y, width, height, BLACK(255));
			//g_pRender->BorderedBox(Left + 1, Top.y + 1, width - 2, height - 2, ColorElements.Color_Weapon->color);
		//	g_pRender->BorderedBox(Left + 2, Top.y + 2, width - 4, height - 4, BLACK(255));
			g_Draw.Box(Left, Top.y, width, height,0 , 0, 0, 255);
			g_Draw.Box(Left + 1, Top.y + 1, width - 2, height - 2, getR(ColorElements.Color_Weapon->color), getG(ColorElements.Color_Weapon->color), getB(ColorElements.Color_Weapon->color), 255);
			g_Draw.Box(Left + 2, Top.y + 2, width - 4, height - 4, 0, 0, 0, 255);
		}
		if (VisualElements.Visual_CBaseEntity_Name->Checked)
		{
			g_Draw.StringA(g_Draw.font_esp, true, Left + width / 2, Bot.y, 200, 200, 200, 255,  name.c_str());
		}	
	}
}

void CESP::Update()
{
	

	if (!G::LocalPlayer)
		return;

	if (!VisualElements.Visual_Player_Active->Checked)
		return;

	for (int i = 0; i < 64; i++)
	{
		this->player_draw_array[i].render = false;
		this->player_draw_array[i].pPlayer = nullptr;

		

		CBaseEntity* pBaseEntity = g_pEntitylist->GetClientEntity(i);
		if (!pBaseEntity)
			continue;

			int index = pBaseEntity->Index();



			if (pBaseEntity->IsDormant() || pBaseEntity->GetHealth() < 1)
			{
			
				continue;
			}


			bool is_friendly = pBaseEntity->GetTeamNum() == G::LocalPlayer->GetTeamNum();

			if (pBaseEntity == G::LocalPlayer)
				if (!G::InThirdperson || !VisualElements.Visual_Misc_SelfESP->Checked)
					continue;

			if (!is_friendly || is_friendly && !VisualElements.Visual_Player_EnemyOnly->Checked || pBaseEntity == G::LocalPlayer)
			{
				bool isEnemyVis = g_pEngineTrace->IsVisible(G::LocalPlayer, G::LocalPlayer->GetEyePosition(), pBaseEntity->GetEyePosition(), pBaseEntity);

				Vector Bot, Top;
				float Left, Right;

				GetCBaseEntityBox(pBaseEntity, Bot, Top, Left, Right, true);

				int height = Bot.y - Top.y;
				int width = Right - Left;
				int x = Left;
				int y = Top.y;

				DWORD col;
				if (pBaseEntity->GetTeamNum() == 2/*terrorists*/)
					col = isEnemyVis ? ColorElements.Color_T_Visible->color : ColorElements.Color_T_Invisible->color;
				else if (pBaseEntity->GetTeamNum() == 3/*pBaseEntity->GetTeamNum() == pLocalCBaseEntity->GetTeamNum()*/)
					col = isEnemyVis ? ColorElements.Color_CT_Visible->color : ColorElements.Color_CT_Invisible->color;



				this->player_draw_array[index].render = true;
				this->player_draw_array[index].color = col;
				this->player_draw_array[index].pPlayer = pBaseEntity;
				this->player_draw_array[index].is_visible = isEnemyVis;
				this->player_draw_array[index].screen_x = x;
				this->player_draw_array[index].screen_y = y;
				this->player_draw_array[index].width = width;
				this->player_draw_array[index].height = height;

				if (VisualElements.Visual_Player_Bones->Checked)
				{
					DrawBones(pBaseEntity, 255, 255, 255, 255);//col.r(), col.g(), col.b(), 255);
				}
			}
		}
}
void CESP::Draw()
{
	if (!VisualElements.Visual_Player_Active->Checked)
		return;

	
	
	for (int i = 0; i < 64; i++)
	{
		if (!this->player_draw_array[i].render || !this->player_draw_array[i].pPlayer)
			continue;

		CBaseEntity* pPlayer =this->player_draw_array[i].pPlayer;
		if (!pPlayer)
			continue;

		if (i != pPlayer->Index())
			continue;

		int x = this->player_draw_array[i].screen_x;
		int y = this->player_draw_array[i].screen_y;
		int width = this->player_draw_array[i].width;
		int height = this->player_draw_array[i].height;
	//	bool is_visible = draw_array.at(i).is_visible;
		DWORD color = this->player_draw_array[i].color;

		CBaseCombatWeapon* pMainWeapon = pPlayer->GetWeapon();
		if (VisualElements.Visual_Player_Bones->Checked)
		{
			//DrawBones(pPlayer, 255, 255, 255, 255);//col.r(), col.g(), col.b(), 255);
		}
		if (VisualElements.Visual_Player_Snaplines->Checked && pPlayer != G::LocalPlayer)
		{
			g_pRender->Line((G::Screen.width / 2) + 1, G::Screen.height, x + width / 2 + 1, y + height, BLACK(255));
			g_pRender->Line(G::Screen.width / 2, G::Screen.height, x + width / 2, y + height, color);
			g_pRender->Line((G::Screen.width / 2) - 1, G::Screen.height, x + width / 2 - 1, y + height, BLACK(255));
		}
		if (VisualElements.Visual_Player_Grenade_Prediction->Checked && pPlayer != G::LocalPlayer)
		{
			if (pMainWeapon && pMainWeapon->IsGrenade())
			{
				PredictNade(pPlayer, pPlayer->GetEyeAngles());
			}
		}
		if (pPlayer == G::LocalPlayer)
		{
			/*show lby and shit*/
			Vector vecLby, vecReal, vecFake;
			Vector org = G::LocalPlayer->GetOrigin(), vOrg;
			Vector end, vEnd;
			Math::AngleVectors(Vector(0, Math::NormalizeYaw(G::LocalPlayer->LowerBodyYaw()), 0), &vecLby);
			Math::AngleVectors(Vector(0, Math::NormalizeYaw(G::RealAngle.y), 0), &vecReal);
			Math::AngleVectors(Vector(0, Math::NormalizeYaw(G::FakeAngle.y), 0), &vecFake);


			
			end = org + (vecFake * 70);

			if (GameUtils::WorldToScreen(org, vOrg) && GameUtils::WorldToScreen(end, vEnd))
			{
				g_pRender->Line(vOrg.x, vOrg.y, vEnd.x, vEnd.y, D3DCOLOR_RGBA(0, 255, 0, 255));
			}
			end = org + (vecReal * 70);

			if (GameUtils::WorldToScreen(org, vOrg) && GameUtils::WorldToScreen(end, vEnd))
			{
				g_pRender->Line(vOrg.x, vOrg.y, vEnd.x, vEnd.y, D3DCOLOR_RGBA(255, 0, 0, 255));
			}

			end = org + (vecLby * 70);

			if (GameUtils::WorldToScreen(org, vOrg) && GameUtils::WorldToScreen(end, vEnd))
			{
				g_pRender->Line(vOrg.x, vOrg.y, vEnd.x, vEnd.y, D3DCOLOR_RGBA(0, 0, 255, 255));
			}
		}
		if (VisualElements.Visual_Player_DLight->Checked)
		{
			dlight_t* pDlight = g_pEffects->CL_AllocDlight(pPlayer->Index());

			pDlight->origin = pPlayer->GetOrigin();
			pDlight->radius = 75;
			pDlight->color = color; pDlight->color.SetAlpha(10);
			pDlight->die = g_pGlobals->curtime + 0.05f;
			pDlight->decay = pDlight->radius / 5.0f;
			pDlight->key = pPlayer->Index();


			dlight_t* pElight = g_pEffects->CL_AllocElight(pPlayer->Index());
			pElight->origin = pPlayer->GetOrigin() + Vector(0.0f, 0.0f, 35.0f);
			pElight->radius = 75;
			pElight->color = color; pElight->color.SetAlpha(10);
			pElight->die = g_pGlobals->curtime + 0.05f;
			pElight->decay = pElight->radius / 5.0f;
			pElight->key = pPlayer->Index();
		}

		if (VisualElements.Visual_Player_Box->Checked)
		{
			///g_pRender->BorderedBoxOutlined(x, y, width, height, color, BLACK(255));
			g_pRender->BorderedBox(x+1, y+1, width-2, height-2, color);
			g_pRender->BorderedBox(x, y, width, height, BLACK(255));
		}
		if (VisualElements.Visual_Player_Armor->Checked)
		{
			if (pPlayer->GetArmor() > 0)
			{
				int armor = 100 - pPlayer->GetArmor();

				int w = 4;
				if (width < 4)
					w = width;


				//int distance = width / 8;


				g_pRender->FilledBox(x + width /*+ distance*/, y + armor*height / 100, w, pPlayer->GetArmor()*height / 100, D3DCOLOR_RGBA(18, 43, 255, 255));

				g_pRender->BorderedBox(x + width /*+ distance*/, y, w, height, BLACK(255));
			}
		}
		if (VisualElements.Visual_Player_Health->Checked)
		{
			int health = 100 - pPlayer->GetHealth();
			int w = 4;
			if (width < 4)
				w = width;

			int hr, hg, hb;


			hr = 255 - (pPlayer->GetHealth()*2.55);
			hg = pPlayer->GetHealth() * 2.55;
			hb = 0;

			//int distance = width / 8;

			g_pRender->FilledBox(x - (w)/*-distance*/, y + health*height / 100, w, pPlayer->GetHealth()*height / 100, D3DCOLOR_RGBA(hr, hg, hb, 255));

			g_pRender->BorderedBox(x - (w)/*-distance*/, y, w, height, BLACK(255));

			if (pPlayer->GetHealth() < 100)
			{
				g_pRender->String(x - w /*-distance*/, y + health*height / 100 - 12, centered, g_pRender->Fonts.esp, true, WHITE(255), "%i HP", pPlayer->GetHealth());
			}
		}
		int bot_add = 0;
		int top_add = 0;
		if (VisualElements.Visual_Player_Name->Checked)
		{
			player_info_s info;
			g_pEngine->GetPlayerInfo(pPlayer->Index(), &info);
			g_pRender->String(x + width / 2, y - 12 - top_add, centered, g_pRender->Fonts.esp, true, WHITE(255), "%s", info.m_szPlayerName);
			top_add += 12;
		}
		if (VisualElements.Visual_Player_Weapon->Checked)
		{
			UINT* hWeapons = pPlayer->GetWeapons();
			if (hWeapons)
			{
				//0 is knife
				//1 is main weapon //pistol if no primary
				//2 is secondary
				//3 is bomb
				//4 is active weapon or the first index after the only weapon you have
				//grenades get added aswell so the numbers above only account for players without grenades

				if (pMainWeapon)
				{
					std::string s1 = WeaponIDName(pMainWeapon->WeaponID());
					if (VisualElements.Visual_Player_Weapon_Ammo->Checked && pMainWeapon->Clip1() != -1 && pMainWeapon->GetWeaponType() != WEAPONCLASS::WEPCLASS_INVALID)
					{
						s1.append(XorStr(" | "));
						s1.append(to_string(pMainWeapon->Clip1()));
					}

					g_pRender->String(x + width / 2, y + height + bot_add, centered, g_pRender->Fonts.esp, true, WHITE(255), "%s", s1.c_str());

					bot_add += 12;
					if (VisualElements.Visual_Player_Weapon_All->Checked)
					{
						for (int nIndex = 0; hWeapons[nIndex]; nIndex++) //so we wanna start with 1 and do not use the primary
						{
							CBaseCombatWeapon* pWeapon = (CBaseCombatWeapon*)g_pEntitylist->GetClientEntityFromHandle(hWeapons[nIndex]);
							if (!pWeapon || pWeapon == pMainWeapon || pWeapon->GetWeaponType() == WEAPONCLASS::WEPCLASS_KNIFE)
								continue;

							std::string s = WeaponIDName(pWeapon->WeaponID());
							if (VisualElements.Visual_Player_Weapon_Ammo->Checked&& pWeapon->Clip1() != -1 && pWeapon->GetWeaponType() != WEAPONCLASS::WEPCLASS_INVALID)
							{
								s.append(XorStr(" | "));
								s.append(to_string(pWeapon->Clip1()));
							}
							g_pRender->String(x + width / 2, y + height + bot_add, centered, g_pRender->Fonts.esp, true, WHITE(255), "%s", s.c_str());

							bot_add += 12;
						}
					}
				}
			}
		}
		
	}

	
}
void CESP::Loop()
{
	if (!G::LocalPlayer)
		return;

	auto HitChance = [](CBaseEntity* pCSTargetCBaseEntity, float range, uint32_t chance)
	{
		if (!G::LocalPlayer ||G::LocalPlayer->GetHealth() < 0|| !G::MainWeapon)
			return false;
		int iHits = 0;
		int iHitsNeed = static_cast<int>(256.f * (static_cast<float>(chance) / 100.f));
		//Vector vTarget = pCSTargetCBaseEntity->GetOrigin();
		Vector vDir, vSpread, vForward, vRight, vUp;
		Vector vLocalEye = G::LocalPlayer->GetEyePosition();
		//Vector vEnemyPos(vTarget.x, vTarget.y, vTarget.z);
		//float flDistance = Vector(vEnemyPos - vLocalEye).Length();
		bool bHitchance = false;

		trace_t tr;
		Ray_t ray;
		CTraceCBaseEntity filter;

		//filter.pHit = pCSTargetCBaseEntity;

		for (int i = 0; i<256; i++)
		{
			vSpread = g_NoSpread->SpreadFactor(i+1);

			Vector vViewAngle;

			vViewAngle = vSpread;

			Math::AngleVectors(vViewAngle, &vForward, &vRight, &vUp);

			auto vEnd = vLocalEye + (vForward * (8000 + 256.f)); //alternative:scale to max range




			ray.Init(G::LocalPlayer->GetEyePosition(), vEnd);
			g_pEngineTrace->TraceRay_NEW(ray, MASK_SHOT, &filter, &tr);
			
			g_pDebugOverlay->AddLineOverlay(tr.startpos, tr.endpos, 255, 255, 255, false, g_pGlobals->interval_per_tick * 3);

			
		}
		return bHitchance;
	};

	
	if (G::LocalPlayer && G::LocalPlayer->GetHealth() > 0 && G::MainWeapon)
	{
		if (G::MainWeapon->IsGrenade())
		{
			QAngle angle;
			g_pEngine->GetViewAngles(angle);
			PredictNade(G::LocalPlayer, angle);
		}
		
		
	}
	
	for (int i = g_pEntitylist->GetHighestEntityIndex() - 1; i >= 1; i--)
	{
		CBaseEntity* pBaseEntity = g_pEntitylist->GetClientEntity(i);
		if (!pBaseEntity)
			continue;

		ClientClass* pClass = (ClientClass*)pBaseEntity->GetClientClass();
		if (pClass->m_ClassID == int(EClassIds::CCSPlayer) && VisualElements.Visual_Player_Active->Checked)
		{
			if (pBaseEntity->IsDormant() || pBaseEntity->GetHealth() < 1)
				continue;

			bool is_friendly = pBaseEntity->GetTeamNum() == G::LocalPlayer->GetTeamNum();

			if (pBaseEntity == G::LocalPlayer)
				if (!G::InThirdperson || !VisualElements.Visual_Misc_SelfESP->Checked)
					continue;
			//
			//continue;


			if (!is_friendly || is_friendly && !VisualElements.Visual_Player_EnemyOnly->Checked || pBaseEntity == G::LocalPlayer)
				g_ESP->DrawPlayer(pBaseEntity, G::LocalPlayer);
		}
		if (VisualElements.Visual_CBaseEntity_Active->Checked)
		{
			Vector buf, pos = pBaseEntity->GetOrigin();
			if (pos.x == 0 || pos.y == 0 || pos.z == 0 || !GameUtils::WorldToScreen(pos, buf))
				continue;


			if ((pClass->m_ClassID != int(EClassIds::CBaseWeaponWorldModel) && (strstr(pClass->m_pNetworkName, XorStr("Weapon")) || pClass->m_ClassID == int(EClassIds::CDEagle) || pClass->m_ClassID == int(EClassIds::CAK47))) && VisualElements.Visual_CBaseEntity_Weapons->Checked)
				DrawDroppedWeapon((CBaseCombatWeapon*)pBaseEntity);

			if ((pClass->m_ClassID == int(EClassIds::CC4) || pClass->m_ClassID == int(EClassIds::CPlantedC4)))
			{
				if (VisualElements.Visual_CBaseEntity_C4->Checked)
					DrawC4(pBaseEntity, pClass->m_ClassID == int(EClassIds::CPlantedC4), G::LocalPlayer);
			}

			if (pClass->m_ClassID == int(EClassIds::CHostage) && VisualElements.Visual_CBaseEntity_Hostages->Checked)
				DrawHostage(pBaseEntity);

			if (strstr(pClass->m_pNetworkName, XorStr("Projectile")) && VisualElements.Visual_CBaseEntity_Grenades->Checked)
				DrawThrowable(pBaseEntity);
		}
	}
	
	if (!g_pGlowObjectManager)
		return;


	CGlowObjectManager* GlowObjectManager = (CGlowObjectManager*)g_pGlowObjectManager;
	CGlowObjectManager::GlowObjectDefinition_t* glowCBaseEntity;



	for (int i = 0; i < GlowObjectManager->size; i++)
	{
		glowCBaseEntity = &GlowObjectManager->m_GlowObjectDefinitions[i];
		CBaseEntity* CBaseEntity = glowCBaseEntity->getCBaseEntity();

		if (!CBaseEntity)
			continue;

		ClientClass* cClass = (ClientClass*)CBaseEntity->GetClientClass();

		if (cClass->m_ClassID == (int)CSGOClassID::CCSPlayer && VisualElements.Visual_Player_Glow->Checked && VisualElements.Visual_Player_Active->Checked)
		{
			int r, g, b, a;

			if (CBaseEntity->IsDormant() || CBaseEntity->GetHealth() < 1)
				continue;


			if (CBaseEntity != G::LocalPlayer)
				if (VisualElements.Visual_Player_EnemyOnly->Checked && CBaseEntity->GetTeamNum() == G::LocalPlayer->GetTeamNum())
					continue;


			bool isEnemyVis = g_pEngineTrace->IsVisible(G::LocalPlayer, G::LocalPlayer->GetEyePosition(), CBaseEntity->GetEyePosition(), CBaseEntity);

			DWORD col;

			if (CBaseEntity->GetTeamNum() == 2/*terrorists*/)
				col = isEnemyVis ? ColorElements.Color_T_Visible->color : ColorElements.Color_T_Invisible->color;
			else if (CBaseEntity->GetTeamNum() == 3/*pBaseEntity->GetTeamNum() == pLocalCBaseEntity->GetTeamNum()*/)
				col = isEnemyVis ? ColorElements.Color_CT_Visible->color : ColorElements.Color_CT_Invisible->color;

			Color color; color.SetAlpha(255);
			color.SetColor(getR(col), getG(col), getB(col));
			glowCBaseEntity->set(color);
		}
		if (VisualElements.Visual_CBaseEntity_Active->Checked && VisualElements.Visual_CBaseEntity_Glow->Checked)
		{
			if (cClass->m_ClassID != (int)CSGOClassID::CBaseWeaponWorldModel && ((strstr(cClass->m_pNetworkName, "Weapon") || cClass->m_ClassID == (int)CSGOClassID::CDEagle || cClass->m_ClassID == (int)CSGOClassID::CAK47)) && VisualElements.Visual_CBaseEntity_Weapons->Checked)
				glowCBaseEntity->set(Color(getR(ColorElements.Color_Weapon->color), getG(ColorElements.Color_Weapon->color), getB(ColorElements.Color_Weapon->color), 255));
			else if (cClass->m_ClassID == (int)CSGOClassID::CPlantedC4 && VisualElements.Visual_CBaseEntity_C4->Checked)
				glowCBaseEntity->set(Color(getR(ColorElements.Color_Planted_Bomb->color), getG(ColorElements.Color_Planted_Bomb->color), getB(ColorElements.Color_Planted_Bomb->color), 255));
			else if (cClass->m_ClassID == (int)CSGOClassID::CC4&& VisualElements.Visual_CBaseEntity_C4->Checked)
				glowCBaseEntity->set(Color(getR(ColorElements.Color_Bomb->color), getG(ColorElements.Color_Bomb->color), getB(ColorElements.Color_Bomb->color), 255));
			else if (cClass->m_ClassID == (int)CSGOClassID::CHostage && VisualElements.Visual_CBaseEntity_Hostages->Checked)
				glowCBaseEntity->set(Color(getR(ColorElements.Color_Hostage->color), getG(ColorElements.Color_Hostage->color), getB(ColorElements.Color_Hostage->color), 255));


		}
	}
}
void CESP::DrawScope(CBaseEntity* pLocalPlayer)
{
	if (VisualElements.Visual_Misc_NoScope->Checked)
	{
		if (pLocalPlayer && pLocalPlayer->GetHealth() > 0)
		{
			CBaseCombatWeapon* pWeapon = pLocalPlayer->GetWeapon();
			if (pWeapon && pLocalPlayer->IsScoped())
			{

					int id = pWeapon->WeaponID();
					g_pRender->Line(g_pRender->Screen.x_center, g_pRender->Screen.Height, g_pRender->Screen.x_center, 0, BLACK(255));
					g_pRender->Line(0, g_pRender->Screen.y_center, g_pRender->Screen.Width, g_pRender->Screen.y_center, BLACK(255));
			}
		}
	}
}