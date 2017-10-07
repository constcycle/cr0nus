#include "sdk.h"
#include "controls.h"
#include "Draw.h"
#include "Menu.h"

MTab::MTab(MBaseElement* parent, int number_tabs, std::vector<std::string> TabNames)
{
	this->parent = parent;
	this->size = Vector(parent->size.x - 20, 24);
	this->number_tabs = number_tabs;
	this->Tabs = TabNames;
	parent->AddChildControl(this);
}


void MTab::Draw()
{
	this->position = this->parent->position;
	this->position.x += 10;
	this->position.y += 10;

	this->size.x = parent->size.x - 20;

	int width = size.x - 1;

	for (int i = 0; i < this->parent->Children.size(); i++)
		if (this->parent->Children.at(i) != (MBaseElement*)this)
		{
			this->position.y += this->parent->Children.at(i)->size.y + 5; //boxes are 15 px big, that means 5 difference is natural
		}
		else
			break;

	std::vector<MBaseElement*> BufferVec = this->Children;
	if (this->Children2.size() > BufferVec.size())
		BufferVec = this->Children2;

	int buffersize = 0;
	for (int i = 0; i < BufferVec.size(); i++)
		buffersize += BufferVec.at(i)->size.y;


	this->size.y = 15 + 5 * BufferVec.size() + buffersize;//20 * this->Children.size() + 15; //old version without color pickers


	int width_per_tab = width / number_tabs;

	for (int i = 0; i < number_tabs; i++)
	{
		bool bHover = m_pMenu->MainWindow->InBounds(this->position + Vector(width_per_tab*i, 0, 0), Vector(width_per_tab, 15));

		if (i == this->current_tab)
		{
			g_Draw.FillRGBA(this->position.x + width_per_tab*i, this->position.y, width_per_tab, 15, pWhite.r(), pWhite.g(), pWhite.b(), m_pMenu->alpha);
		}
		else if (bHover)
		{
			if (m_pMenu->MainWindow->bMouse1released)
				current_tab = i;

			g_Draw.FillRGBA(this->position.x + width_per_tab*i, this->position.y, width_per_tab, 15, MidGrey.r(), MidGrey.g(), MidGrey.b(), m_pMenu->alpha);
		}
		else
			g_Draw.FillRGBA(this->position.x + width_per_tab*i, this->position.y, width_per_tab, 15, LightGrey.r(), LightGrey.g(), LightGrey.b(), m_pMenu->alpha);

		g_Draw.StringA(g_Draw.font_menu, true, this->position.x + width_per_tab*i + width_per_tab / 2, this->position.y, 255, 255, 255, m_pMenu->alpha, "%s", Tabs.at(i).c_str());
	}

	this->position.x -= 10;
	this->position.y += 10;

	this->size.x += 20;

	if (current_tab == 0)
	{
		for (int i = Children.size() - 1; i >= 0; i--)
			Children[i]->Draw();
	}
	else if (current_tab == 1)
	{
		for (int i = Children2.size() - 1; i >= 0; i--)
			Children2[i]->Draw();
	}

	this->position.x += 10;
}