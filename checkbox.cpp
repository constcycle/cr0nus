#include "sdk.h"
#include "controls.h"
#include "Render.h"
#include "Menu.h"
MCheckbox::MCheckbox(MBaseElement* parent, std::string name, std::string caption, MCategory* SaveCategory, std::string savename)
{
	this->parent = parent;
	this->name = name;
	this->size = Vector(g_pRender->menu_control_size, g_pRender->menu_control_size);
	this->caption = caption;
	this->FileIdentifier = savename;
	this->FileControlType = UIControlTypes::UIC_CheckBox;
	this->in_tab = in_tab;
	/*Set save parent array*/

	parent->AddChildControl(this);
	

	if(SaveCategory != nullptr)
		SaveCategory->SaveChildren.emplace_back(this);
}

void MCheckbox::Draw()
{
	
	
		this->position = this->parent->position;
		this->position.x += 10;
		this->position.y += 10;

		/*get parent tab*/
		MTab* pTab = (MTab*)parent;


		for (int i = 0; i < this->parent->Children.size(); i++)
			if (this->parent->Children.at(i) != (MBaseElement*)this)
			{
				this->position.y += this->parent->Children.at(i)->size.y + 5; //boxes are 15 px big, that means 5 difference is natural
			}
			else
				break;

	
		bool bHovering = false;

	if (!m_pMenu->MainWindow->BlockInput)
	{
		bHovering = m_pMenu->MainWindow->InBounds(this->position, Vector(this->size.y, this->size.y));
		if (m_pMenu->MainWindow->Clicked(this->position, Vector(this->size.x, this->size.y)))
			if (!this->Checked)
				this->Checked = true;
			else
				this->Checked = false;
	}

	int alpha = m_pMenu->alpha;
	

	if (this->Checked)
		g_pRender->GradientBoxOutlined(this->position.x, this->position.y, this->size.x, this->size.y, 1,D3DCOLOR_ARGB(alpha, 0, 162, 232), D3DCOLOR_ARGB(alpha, 0, 112, 182), BLACK(alpha),true);
	else
	{
		
			if (bHovering)
			{
				g_pRender->FilledBoxOutlined(this->position.x, this->position.y, this->size.x, this->size.y,  D3DCOLOR_ARGB(alpha, 0,122, 174), BLACK(alpha));
				//g_pRender->GradientBoxOutlined(this->position.x, this->position.y, this->size.x, this->size.y, 1, D3DCOLOR_ARGB(alpha, 0, 122, 174), D3DCOLOR_ARGB(alpha, 0, 92, 144), BLACK(alpha), false);
				//g_Draw.StringA(g_Draw.font_menu, true, m_pMenu->MainWindow->position.x + m_pMenu->MainWindow->size.x / 2, m_pMenu->MainWindow->position.y + m_pMenu->MainWindow->size.y + m_pMenu->MainWindow->dragYoffset + m_pMenu->MainWindow->dragYoffset / 2-6, 255, 255, 255, m_pMenu->alpha, "%s", this->caption.c_str());
			}
			else
			{
				g_pRender->FilledBoxOutlined(this->position.x, this->position.y, this->size.x, this->size.y, D3DCOLOR_ARGB(alpha, 69, 69, 69), BLACK(alpha));
			}
		
	}
	
	//g_Draw.StringA(g_Draw.font_menu, false, this->position.x + 20, this->position.y, 255, 255, 255, m_pMenu->alpha, "%s", this->name.c_str());

	g_pRender->Text((char*)this->name.c_str(), this->position.x + 20, this->position.y, lefted, g_pRender->Fonts.menu_control, true, WHITE(alpha), BLACK(alpha));
}