#pragma once
#include <string>
#include <Windows.h>
#include <vector>
#include "controls.h"
using namespace std;
/*Include menu elements*/
extern CRagebotElements RagebotElements;
extern CMiniWindows MiniWindowElements;
extern CVisualElements VisualElements;
extern CMiscElements MiscElements;
extern CColorElements ColorElements;

class CMenu
{
public:
	CMenu();
	//void InitConfig(HMODULE hModule);
	//void InitSkins();
	void SaveWindowState(std::string Filename);
	void LoadWindowState(std::string Filename);
	//void ReadOrWriteConfig(const char* szPath, bool bRead);
	float alpha = 0;
	MWindow* MainWindow = nullptr;
	bool initialized = false;

}; extern CMenu* m_pMenu;



