#include "sdk.h"
#include "controls.h"
#include "Menu.h"
#include "Render.h"
MDropdown::MDropdown(MBaseElement* parent, std::string name, std::string caption, MCategory* SaveCategory, std::string savename, std::vector<string> Items)
{
	this->parent = parent;
	this->name = name;
	this->size = Vector(parent->size.x - 20, 31);
	this->caption = caption;
	this->FileIdentifier = savename;
	this->FileControlType = UIControlTypes::UIC_DropDown;
	this->Items = Items;
	
	/*Set save parent array*/

	
		parent->AddChildControl(this);


	if(SaveCategory != nullptr)
		SaveCategory->SaveChildren.emplace_back(this);
}

bool m_bActiveComboBox = false;
void MDropdown::Draw()
{
	
	
		this->position = this->parent->position;
		this->position.x += 10;
		this->position.y += 10;

		this->size.x = parent->size.x - 20;
	

	MTab* pTab = (MTab*)parent;

	
		for (int i = 0; i < this->parent->Children.size(); i++)
			if (this->parent->Children.at(i) != (MBaseElement*)this)
			{
				this->position.y += this->parent->Children.at(i)->size.y + 5; //boxes are 15 px big, that means 5 difference is natural
			}
			else
				break;
	

	auto nSize = Items.size();
	
	int alpha = m_pMenu->alpha;

	
	g_pRender->Text((char*)name.c_str(), position.x, position.y, lefted, g_pRender->Fonts.menu_control, true, WHITE(alpha), BLACK(alpha));
	
	int bar_height = 17;
	int y_pos = this->position.y + 12;
	int width = this->size.x;

	bool bHovering = m_pMenu->MainWindow->InBounds(Vector(position.x, y_pos), Vector(width, bar_height));
	if (m_pMenu->MainWindow->BlockInput && m_pMenu->MainWindow->BlockingElement != (MBaseElement*)this)
		bHovering = false;
	if (!m_pMenu->MainWindow->BlockInput || m_pMenu->MainWindow->BlockInput && m_pMenu->MainWindow->BlockingElement == this)
	{
		if (bHovering && m_pMenu->MainWindow->bMouse1released && !m_bActiveComboBox && !IsOpen)
		{
			IsOpen = true;
			m_bActiveComboBox = !m_bActiveComboBox;
		}
	}
	
	
	//if (IsOpen)
		
	if (bHovering || IsOpen)
		g_pRender->FilledBoxOutlined(this->position.x, y_pos, width, bar_height, D3DCOLOR_ARGB(alpha, 35, 35, 35), BLACK(alpha));
	else
		g_pRender->GradientBoxOutlined(this->position.x, y_pos, width, bar_height, 1, D3DCOLOR_ARGB(alpha, 45, 45, 45), D3DCOLOR_ARGB(alpha, 25, 25, 25), BLACK(alpha), true);
	

	
	//if (bHovering)
		//g_Draw.StringA(g_Draw.font_menu, true, m_pMenu->MainWindow->position.x + m_pMenu->MainWindow->width / 2, m_pMenu->MainWindow->position.y + m_pMenu->MainWindow->size.y + m_pMenu->MainWindow->dragYoffset + m_pMenu->MainWindow->dragYoffset / 2-6, m_pMenu->MainWindow->ActiveFontColor.r(), m_pMenu->MainWindow->ActiveFontColor.g(), m_pMenu->MainWindow->ActiveFontColor.b(), m_pMenu->MainWindow->ActiveFontColor.a(), "%s", this->caption.c_str());
	


	if (Items.size() > 0)
	{
		if (IsOpen)
		{
			m_bActiveComboBox = true;

			m_pMenu->MainWindow->BlockInput = true;
			m_pMenu->MainWindow->BlockingElement = this;
			auto Y = y_pos + bar_height;
			

			for (int i = 0; i < Items.size(); i++)
			{
				
				auto bHover = m_pMenu->MainWindow->InBounds(Vector(position.x, Y + i*bar_height), Vector(size.x, bar_height));
				if (bHover)
					g_pRender->FilledBoxOutlined(position.x, Y + i*bar_height, width, bar_height, D3DCOLOR_ARGB(alpha, 35, 35, 35), D3DCOLOR_ARGB(alpha, 0, 162, 232));
				else
					g_pRender->FilledBox(position.x, Y + i*bar_height, width, bar_height, D3DCOLOR_ARGB(alpha, 35, 35, 35));


				g_pRender->Text((char*)Items[i].c_str(), position.x + width / 2, Y + i*bar_height + bar_height/2 - g_pRender->menu_control_size/2, centered, g_pRender->Fonts.menu_control, true, WHITE(alpha), BLACK(alpha));

				if (bHover && m_pMenu->MainWindow->bMouse1released)
				{
					m_pMenu->MainWindow->BlockInput = false;
					m_pMenu->MainWindow->BlockingElement = nullptr;
					m_bActiveComboBox = false;
					IsOpen = false;
					SelectedIndex = i;
				}
				if (m_pMenu->MainWindow->bMouse1pressed && !m_pMenu->MainWindow->InBounds(Vector(position.x, Y), Vector(width, bar_height * Items.size())))
				{
					IsOpen = false;
					m_bActiveComboBox = false;
					m_pMenu->MainWindow->BlockInput = false;
					m_pMenu->MainWindow->BlockingElement = nullptr;
				}
			}
			g_pRender->BorderedBox(this->position.x, Y, width, bar_height*nSize, BLACK(alpha));
		}
	}
	

	if(this->SelectedIndex >= 0 && this->SelectedIndex < Items.size())
		g_pRender->Text((char*)Items[this->SelectedIndex].c_str(), position.x + width / 2, y_pos + bar_height/2 - g_pRender->menu_control_size / 2, centered, g_pRender->Fonts.menu_control, true, WHITE(alpha), BLACK(alpha));

}