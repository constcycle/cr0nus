#include "sdk.h"
#include "hooks.h"
#include "Render.h"
#include "Menu.h"
#include "global.h"
#include "Hitmarker.h"
#include "ESP.h"
#include "Crosshair.h"
HRESULT __stdcall Hooks::D3D9_EndScene(IDirect3DDevice9* pDevice)
{
	HRESULT result = d3d9VMT->GetOriginalMethod<EndSceneFn>(42)(pDevice);
	

	static bool init = false;
	if (!init)
	{
		g_pRender->Init(pDevice);
		init = true;

	
		Sleep(150);

		m_pMenu = new CMenu();
	}
	g_pRender->Reset();

	
	if (g_pEngine->IsInGame() && g_pEngine->IsConnected() && G::LocalPlayer)
	{
		//g_ESP->Draw();
		g_ESP->DrawScope(G::LocalPlayer);

		g_Hitmarker->draw();

		g_Crosshair->draw();

		
	}


	if (m_pMenu)
	{
		
		m_pMenu->MainWindow->Draw();
		if(MiscElements.Misc_Windows_Playerlist->Checked)
			MiniWindowElements.Playerlist->Draw();
		if(MiscElements.Misc_Windows_Skinchanger->Checked)
			MiniWindowElements.Skinchanger->Draw();
		if (MiscElements.Misc_Windows_Damagelist->Checked)
			MiniWindowElements.Damagelist->Draw();
		if (m_pMenu->MainWindow->visible)
		{
			int x = m_pMenu->MainWindow->Cur.x;
			int y = m_pMenu->MainWindow->Cur.y;
			DWORD col = D3DCOLOR_ARGB(3, 6, 26, 255);
			g_pRender->FilledBox(x + 1, y, 1, 17, col);
			for (int i = 0; i < 11; i++)
				g_pRender->FilledBox(x + 2 + i, y + 1 + i, 1, 1, col);
			g_pRender->FilledBox(x + 8, y + 12, 5, 1, col);
			g_pRender->FilledBox(x + 8, y + 13, 1, 1, col);
			g_pRender->FilledBox(x + 9, y + 14, 1, 2, col);
			g_pRender->FilledBox(x + 10, y + 16, 1, 2, col);
			g_pRender->FilledBox(x + 8, y + 18, 2, 1, col);
			g_pRender->FilledBox(x + 7, y + 16, 1, 2, col);
			g_pRender->FilledBox(x + 6, y + 14, 1, 2, col);
			g_pRender->FilledBox(x + 5, y + 13, 1, 1, col);
			g_pRender->FilledBox(x + 4, y + 14, 1, 1, col);
			g_pRender->FilledBox(x + 3, y + 15, 1, 1, col);
			g_pRender->FilledBox(x + 2, y + 16, 1, 1, col);
			for (int i = 0; i < 4; i++)
				g_pRender->FilledBox(x + 2 + i, y + 2 + i, 1, 14 - (i * 2), D3DCOLOR_RGBA(255 - (i * 4), 255 - (i * 4), 255 - (i * 4), 255));
			g_pRender->FilledBox(x + 6, y + 6, 1, 8, D3DCOLOR_RGBA(235, 235, 235, 255));
			g_pRender->FilledBox(x + 7, y + 7, 1, 9, D3DCOLOR_RGBA(231, 231, 231, 255));
			for (int i = 0; i < 4; i++)
				g_pRender->FilledBox(x + 8 + i, y + 8 + i, 1, 4 - i, D3DCOLOR_RGBA(227 - (i * 4), 227 - (i * 4), 227 - (i * 4), 255));
			g_pRender->FilledBox(x + 8, y + 14, 1, 4, D3DCOLOR_RGBA(207, 207, 207, 255));
			g_pRender->FilledBox(x + 9, y + 16, 1, 2, D3DCOLOR_RGBA(203, 203, 203, 255));
		}
	}
	return result;
}