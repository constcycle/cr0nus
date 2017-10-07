#pragma once
class CHitmarker
{
public:
	float end_time;
	void draw();
	void play_sound();
	void update_end_time();
}; extern CHitmarker* g_Hitmarker;