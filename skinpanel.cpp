#include "sdk.h"
#include "controls.h"
#include "Render.h"
#include "Menu.h"

MSkinchanger* GlobalSkinchanger;
MSkinchanger::MSkinchanger(MBaseElement* parent)
{
	this->parent = parent;
	this->parent->AddChildControl(this);

	Skinchanger_Knife = new MDropdown(this, "Knife", "Select the knife you want", nullptr, "", std::vector<string>{"Default", "Karambit", "Bayonet", "M9 Bayonet", "Huntsman", "Gut", "Falchion", "Daggers", "Butterfly", "Flip", "Bowie"});
	Skinchanger_Glove = new MDropdown(this, "Glove", "Select the glove you want", nullptr, "", std::vector<string>{"Default", "Bloodhound | Charred", "Bloodhound | Snakebite", "Bloodhound | Bronzed", "Bloodhound | Guerrilla", "Wraps | Leather", "Wraps | Spruce DDPAT", "Wraps | Slaughter", "Wraps | Badlands", "Driver | Lunar Weave", "Driver | Convoy", "Driver | Crimson Weave", "Driver | Diamondback", "Sport | Hedge Maze", "Sport | Pandoras Box", "Sport | Superconductor", "Sport | Arid", "Moto | Eclipse", "Moto | Spear Mint", "Moto | Boom!", "Moto | Cool Mint", "Specialist | Forest DDPAT", "Specialist | Crimson Kimono", "Specialist | Emerald Web", "Specialist | Foundation" });
	Skinchanger_Wear = new MSlider(this, "Wear", "Activates skinchanger", 0, 1, false, nullptr, "");
	Skinchanger_Seed = new MSlider(this, "Seed", "Activates skinchanger",0, 9999999,true,  nullptr, "");
	Skinchanger_Stattrack = new MCheckbox(this, "Stattrack", "Activates skinchanger", nullptr, "");
	Skinchanger_Stattrack_Kills = new MSlider(this, "Stattrack Kills", "Activates skinchanger", 0, 1337, true, nullptr, "");
}
void MSkinchanger::DrawList1()
{
	int alpha = m_pMenu->alpha;

	int rx = this->position.x + 10;
	int ry = this->position.y+10;
	g_pRender->BorderedBox(rx, ry, 140, 356, BLACK(alpha));
	g_pRender->BorderedBox(rx + 139, ry, 10, 356,BLACK(alpha));

	static int current = 3;
	static int scrollamount = 0;
	static int scry = 0;
	static int bWasntHolding = false;
	static int bGrabbing = false;
	static int iOffsetY = 0;

	static auto& _skininfo = SkinsUtil::pSkins->GetData();
	if (SkinsUtil::data.empty())
	{
		SkinsUtil::ParseSkins();
	}
	int i = 0;
	std::string name;
	for (auto& w : _skininfo)
	{
		if (w.first.find("glove") != std::string::npos)
			continue;
		if (w.first.find("leather") != std::string::npos)
			continue;
		int j = i - scrollamount;
		if (j > -1 && j < 25)
		{
			bool bHover = m_pMenu->MainWindow->InBounds(Vector(rx + 1, ry + 3 + (j * 14), 0), Vector(138, 14, 0));
			if (i == current)
			{
				currentWepName = w.first;
				currentWep = w.second;
				g_pRender->FilledBox(rx + 1, ry + 3 + (j * 14), 138, 14, D3DCOLOR_ARGB(alpha, 0, 162, 232));
			}
			else if (bHover)
			{
				g_pRender->FilledBox(rx + 1, ry + 3 + (j * 14), 138, 14, D3DCOLOR_ARGB(alpha, 0, 122, 174));
			}
			if (bHover && m_pMenu->MainWindow->bMouse1released && !bGrabbing)
				current = i;
			name = w.first;
			if (name.find("weapon") != std::string::npos)
				name = name.substr(7, name.length() - 7);
			if (name.find("bayonet") != std::string::npos && !(name.find("m9") != std::string::npos))
				name = "knife_bayonet";
	
			g_pRender->String(rx + 70, ry +3+ (j * 14), centered, g_pRender->Fonts.menu_control, true, WHITE(alpha), "%s", name.c_str());
		}
		i++;
	}
	POINT Cur = m_pMenu->MainWindow->Cur;
	

	if (m_pMenu->MainWindow->InBounds(Vector(rx + 140, ry + scry + 1, 0), Vector(9, 31, 0)) && m_pMenu->MainWindow->bMouse1pressed && !bGrabbing/* && bWasntHolding*/)
	{
		bGrabbing = true;
		iOffsetY = ry + scry - Cur.y;
	}
	else if (bGrabbing && !m_pMenu->MainWindow->bMouse1pressed)
	{
		bGrabbing = false;
		iOffsetY = 0;
	}

	bWasntHolding = m_pMenu->MainWindow->InBounds(Vector(rx + 140, ry + scry + 1, 0), Vector(9, 31, 0)) && !m_pMenu->MainWindow->bMouse1pressed;

	if (bGrabbing)
	{
		scry = Cur.y + iOffsetY - ry;


		if (scry < 0)
			scry = 0;
		if (scry > 324)
			scry = 324;

		float diff = (324.f / (float(_skininfo.size()) - 25.f));
		scrollamount = scry / diff;

	}

	g_pRender->FilledBox(rx + 140, ry + scry + 1, 9, 31, D3DCOLOR_RGBA(115, 115, 115, 255));
}

void MSkinchanger::DrawList2()
{
	int alpha = m_pMenu->alpha;

	int rx = this->position.x + 10;
	int ry = this->position.y+10;
	int _rx = rx + 166;

	g_pRender->BorderedBox(_rx, ry, 140, 356, BLACK(alpha));
	static int current = 3;
	static int scrollamount = 0;
	static int scry = 0;
	static int bWasntHolding = false;
	static int bGrabbing = false;
	static int iOffsetY = 0;

	static auto& _skininfo = SkinsUtil::pSkins->GetData();
	int i = 0;
	for (auto& w : currentWep)
	{
		std::unordered_map<std::string, int>::iterator data = SkinsUtil::data.find(currentWepName);
		if (data == SkinsUtil::data.end())
			continue;
		int j = i - scrollamount;
		if (j > -1 && j < 25)
		{
			bool bHover = m_pMenu->MainWindow->InBounds(Vector(_rx + 1, ry + 3 + (j * 14), 0), Vector(138, 14, 0));

			if (w._id == data->second)
				g_pRender->FilledBox(_rx + 1, ry + 3 + (j * 14), 138, 14, D3DCOLOR_ARGB(alpha, 0, 162, 232));
			else if(bHover)
				g_pRender->FilledBox(_rx + 1, ry + 3 + (j * 14), 138, 14, D3DCOLOR_ARGB(alpha, 0, 122, 174));
			if (bHover && m_pMenu->MainWindow->bMouse1released && !bGrabbing)
			{
				Reload = true;
				data->second = w._id;
			}
			
			g_pRender->String(_rx + 70, ry + 3 + (j * 14), centered, g_pRender->Fonts.menu_control, true, WHITE(alpha), "%s", w._name.c_str());
		}

		i++;
	}

	if (currentWep.size() <= 25)
	{
		iOffsetY = 0;
		return;
	}

	POINT Cur;
	HWND Window = FindWindow(NULL, "Counter-Strike: Global Offensive");
	GetCursorPos(&Cur);
	ScreenToClient(Window, &Cur);

	g_pRender->BorderedBox(_rx + 139, ry, 10, 356, D3DCOLOR_RGBA(0, 40, 40, 255));



	if (m_pMenu->MainWindow->InBounds(Vector(_rx + 140, ry + scry + 1, 0), Vector(9, 31, 0)) && m_pMenu->MainWindow->bMouse1pressed && !bGrabbing /*&& bWasntHolding*/)
	{
		bGrabbing = true;
		iOffsetY = ry + scry - Cur.y;
	}
	else if (bGrabbing && !GetAsyncKeyState(VK_LBUTTON))
	{
		bGrabbing = false;
		iOffsetY = 0;
	}

	//bWasntHolding = m_pMenu->MainWindow->InBounds(Vector(_rx + 140, ry + scry + 1, 0), Vector(9, 31, 0)) && !GetAsyncKeyState(VK_LBUTTON);

	if (bGrabbing)
	{
		scry = Cur.y + iOffsetY - ry;

		if (scry < 0)
			scry = 0;
		if (scry > 324)
			scry = 324;

		float diff = (324.f / (float(currentWep.size()) - 25.f));
		scrollamount = scry / diff;
	}

	g_pRender->FilledBox(_rx + 140, ry + scry + 1, 9, 31, D3DCOLOR_RGBA(115, 115, 115, 255));
}
void ForceUpdate()
{
	
	
}
void MSkinchanger::Draw()
{
//	if (!MiscElements.Misc_General_Skinchanger->Checked)
		//return;
	this->position.x = parent->position.x;
	this->position.y = parent->position.y +  m_pMenu->MainWindow->dragYoffset/2;
	this->size.y = parent->size.y;
	this->size.x = parent->size.x - 15;
	static int OldKnife = 0;
	static float OldSeed = 0;
	static float OldWear = 0;
	static float OldStattrackKills = 0;
	static bool OldStattrack = false;
	

	if (OldKnife != Skinchanger_Knife->SelectedIndex)
	{
		Reload = true;
		OldKnife = Skinchanger_Knife->SelectedIndex;
	}
	if (OldWear != Skinchanger_Wear->value)
	{
		Reload = true;
		OldWear = Skinchanger_Wear->value;
	}
	if (OldSeed != Skinchanger_Seed->value)
	{
		Reload = true;
		OldSeed = Skinchanger_Seed->value;
	}
	if (OldStattrackKills != Skinchanger_Stattrack_Kills->value)
	{
		Reload = true;
		OldStattrackKills = Skinchanger_Stattrack_Kills->value;
	}
	if (OldStattrack != Skinchanger_Stattrack->Checked)
	{
		Reload = true;
		OldStattrack = Skinchanger_Stattrack->Checked;
	}
	DrawList1();
	DrawList2();

	this->position.y += 360;

	for (int i = Children.size() - 1; i >= 0 ; i--)
		Children[i]->Draw();

	
}