#pragma once
class CCrosshair
{
public:
	void draw_normal();
	void draw_recoil();
	void draw_recoil_spread();
	void draw();
}; extern CCrosshair* g_Crosshair;