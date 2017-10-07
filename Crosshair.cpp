#include "sdk.h"
#include "Crosshair.h"
#include "Render.h"
#include "global.h"
#include "Menu.h"
CCrosshair* g_Crosshair = new CCrosshair;

void CCrosshair::draw_normal()
{
	int x = g_pRender->Screen.x_center;
	int y = g_pRender->Screen.y_center;

	
	g_pRender->Line(x, y + 12, x, y - 12, D3DCOLOR_RGBA(0, 240, 0, 255));
	g_pRender->Line(x + 12, y, x - 12, y, D3DCOLOR_RGBA(0, 240, 0, 255));

	g_pRender->Line(x, y + 7, x, y - 7, D3DCOLOR_RGBA(255, 255, 255, 255));
	g_pRender->Line(x + 7, y, x - 7, y, D3DCOLOR_RGBA(255, 255, 255, 255));
}
void CCrosshair::draw_recoil()
{
	QAngle punchAngle = G::LocalPlayer->GetPunchAngle() * 2;

	int x = g_pRender->Screen.x_center;
	int y = g_pRender->Screen.y_center;
	int dy = g_pRender->Screen.Height / 90;
	int dx = g_pRender->Screen.Width/ 90;

	x -= (dx*(punchAngle.y));
	y += (dy*(punchAngle.x));

	/*Crosshair*/
	g_pRender->Line(x,  y+ 12, x, y - 12, D3DCOLOR_RGBA(0, 240, 0, 255));
	g_pRender->Line(x + 12, y, x - 12, y, D3DCOLOR_RGBA(0, 240, 0, 255));

	g_pRender->Line(x, y + 7, x, y - 7, D3DCOLOR_RGBA(255, 255, 255, 255));
	g_pRender->Line(x + 7, y, x - 7, y, D3DCOLOR_RGBA(255, 255, 255, 255));
}
void CCrosshair::draw_recoil_spread()
{
	QAngle punchAngle = G::LocalPlayer->GetPunchAngle() * 2;

	int x = g_pRender->Screen.x_center;
	int y = g_pRender->Screen.y_center;
	int dy = g_pRender->Screen.Height / 90;
	int dx = g_pRender->Screen.Width / 90;
	x -= (dx*(punchAngle.y));
	y += (dy*(punchAngle.x));


	float spread = (G::MainWeapon->GetSpread() + G::MainWeapon->GetInaccuracy()) * 500;
	g_pRender->Circle(x, y, spread, 32, WHITE(255));
}
void CCrosshair::draw()
{
	switch (MiscElements.Misc_General_Crosshair->SelectedIndex)
	{
	case 0:
		break;
	case 1:
		this->draw_normal();
		break;
	case 2:
		this->draw_recoil();
		break;
	case 3:
		this->draw_recoil_spread();
		break;
	default:
		break;
	}
}