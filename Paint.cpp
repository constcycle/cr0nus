#include "hooks.h"
#include "GameUtils.h"
#include "Menu.h"
StartDrawingFn StartDrawing;
FinishDrawingFn FinishDrawing;

void __stdcall Hooks::Paint(PaintMode_t mode)
{
	enginevguiVMT->GetOriginalMethod<PaintFn>(14)(mode);

	if (!StartDrawing)
	{
		StartDrawing = reinterpret_cast<StartDrawingFn>(Utilities::Memory::FindPattern(XorStr("vguimatsurface.dll"), (PBYTE)XorStr("\x55\x8B\xEC\x83\xE4\xC0\x83\xEC\x38"), XorStr("xxxxxxxxx")));
		Utilities::Log(XorStr("StartDrawing: 0x%X"), (DWORD)StartDrawing);
	}
	if (!FinishDrawing)
	{
		FinishDrawing = reinterpret_cast<FinishDrawingFn>(Utilities::Memory::FindPattern(XorStr("vguimatsurface.dll"), (PBYTE)XorStr("\x8B\x0D\x00\x00\x00\x00\x56\xC6\x05"), XorStr("xx????xxx")));
		Utilities::Log(XorStr("FinishDrawing: 0x%X"), (DWORD)FinishDrawing);
	}
	

	if (mode & PaintMode_t::PAINT_UIPANELS)
	{
		//StartDrawing(g_pSurface);
		//if (m_pMenu)
		//	m_pMenu->MainWindow->Draw();
		//Draw shit here
		//FinishDrawing(g_pSurface);
	}
}
