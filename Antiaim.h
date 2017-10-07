#pragma once
class CAntiaim
{
public:
	bool choke;
	bool lby_update_in_this_tick;
	float lby_updatetime;
	bool EdgeAntiAim(float flWall, float flCornor);
	void Pitch_AA();
	void Yaw_AA();
	void Run(QAngle org_view);
	void AntiAimYaw_Real();
	void AntiAimYaw_Fake();
}; extern CAntiaim* g_Antiaim;