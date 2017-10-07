#include "sdk.h"
#include "hooks.h"
#include "Menu.h"
WNDPROC                            Hooks::g_pOldWindowProc; //Old WNDPROC pointer
HWND                               Hooks::g_hWindow; //Handle to the CSGO window
LRESULT __stdcall Hooks::WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (m_pMenu && m_pMenu->MainWindow && m_pMenu->MainWindow->visible)
		return true;

	return CallWindowProc(Hooks::g_pOldWindowProc, hWnd, uMsg, wParam, lParam);
}