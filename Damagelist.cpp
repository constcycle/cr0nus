#include "sdk.h"
#include "controls.h"
#include "Render.h"
#include "Menu.h"



MDamagelist::MDamagelist(MBaseElement* parent)
{
	this->position = Vector(0, 0, 0);
	this->parent = parent;
	this->parent->AddChildControl(this);
}


void MDamagelist::Draw()
{
	this->position.x = parent->position.x;
	this->position.y = parent->position.y + m_pMenu->MainWindow->dragYoffset / 2;
	this->size.y = parent->size.y;
	this->size.x = parent->size.x - 20;

	//clear all messages if player is not in game
	if (!g_pEngine->IsInGame() && this->damage_infos.size() > 0)
		this->damage_infos.clear();


	int iterations = 0;

	for (int i = this->damage_infos.size() - 1; i >= 0; i--)
	{
		if (i <= 21)
		{
			g_pRender->String(this->position.x + 10, this->position.y + 12 * (iterations), lefted, g_pRender->Fonts.menu_control, true, WHITE(255), "%s", this->damage_infos.at(i).c_str());
			iterations++;
		}
	}
}

MDamagelist* global_damagelist;