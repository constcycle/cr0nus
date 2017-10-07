#pragma once
#include "FixMove.h"
class CMisc
{
public:
	void RankReveal();
	void Bunnyhop();
	void AutoStrafe();
	void FixCmd();
	void FixMovement();
	bool FakeLag();
	std::vector<Vector> path;
	float m_circle_yaw = 0;
}; extern CMisc* g_Misc;