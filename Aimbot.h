#pragma once
class CAimbot
{
public:
	void DropTarget();
	void Run();
	void CompensateInaccuracies();
	void AutoRevolver();
	int target_index = -1;
	float best_distance = 99999.f;
	bool aimbotted_in_current_tick;
	bool fired_in_that_tick;
	float current_aim_simulationtime;
	int current_minusticks;
	Vector current_aim_position;
	Vector current_aim_player_origin;
	CBaseEntity* pTarget;
}; extern CAimbot* g_Aimbot;