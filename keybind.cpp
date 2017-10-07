#include "sdk.h"
#include "controls.h"
#include "Render.h"
#include "Menu.h"
char* KeyStrings[254] = { "Not Bound", "Left Mouse", "Right Mouse", "Control+Break", "Middle Mouse", "Mouse 4", "Mouse 5",
nullptr, "Backspace", "TAB", nullptr, nullptr, nullptr, "ENTER", nullptr, nullptr, "SHIFT", "CTRL", "ALT", "PAUSE",
"CAPS LOCK", nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, "ESC", nullptr, nullptr, nullptr, nullptr, "SPACEBAR",
"PG UP", "PG DOWN", "END", "HOME", "Left", "Up", "Right", "Down", nullptr, "Print", nullptr, "Print Screen", "Insert",
"Delete", nullptr, "0", "1", "2", "3", "4", "5", "6", "7", "8", "9", nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
nullptr, "A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L", "M", "N", "O", "P", "Q", "R", "S", "T", "U", "V", "W", "X",
"Y", "Z", "Left Windows", "Right Windows", nullptr, nullptr, nullptr, "NUM 0", "NUM 1", "NUM 2", "NUM 3", "NUM 4", "NUM 5", "NUM 6",
"NUM 7", "NUM 8", "NUM 9", "*", "+", "_", "-", ".", "/", "F1", "F2", "F3", "F4", "F5", "F6", "F7", "F8", "F9", "F10", "F11", "F12",
"F13", "F14", "F15", "F16", "F17", "F18", "F19", "F20", "F21", "F22", "F23", "F24", nullptr, nullptr, nullptr, nullptr, nullptr,
nullptr, nullptr, nullptr, "NUM LOCK", "SCROLL LOCK", nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
nullptr, nullptr, nullptr, nullptr, nullptr, "LSHIFT", "RSHIFT", "LCONTROL", "RCONTROL", "LMENU", "RMENU", nullptr, nullptr, nullptr,
nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, "Next Track", "Previous Track", "Stop", "Play/Pause", nullptr, nullptr,
nullptr, nullptr, nullptr, nullptr, ";", "+", ",", "-", ".", "/?", "~", nullptr, nullptr, nullptr, nullptr,
nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, "[{", "\\|", "}]", "'\"", nullptr, nullptr, nullptr, nullptr,
nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr };
MKeybind::MKeybind(MBaseElement* parent, std::string name, std::string caption, MCategory* SaveCategory, string savename)
{
	this->parent = parent;
	this->name = name;
	this->caption = caption;
	this->FileIdentifier = savename;
	this->FileControlType = UIControlTypes::UIC_KeyBind;
	/*Set save parent array*/
	parent->AddChildControl(this);
	SaveCategory->SaveChildren.push_back(this);
}
void MKeybind::Draw()
{
	this->position = this->parent->position;
	this->position.x += 10;
	this->position.y += 10;

	this->size.x = parent->size.x - 20;
	this->size.y = 15;

	int index;
	for (int i = 0; i < this->parent->Children.size(); i++)
		if (this->parent->Children.at(i) == (MBaseElement*)this)
			index = i;


	this->position.y += (index) * 20; //boxes are 15 px big, that means 5 difference is natural

	bool bHovering = false;

	


	char* KeyName = "Not Bound";

	
	if (!m_pMenu->MainWindow->BlockInput)
	{
		bHovering = m_pMenu->MainWindow->InBounds(position, size);
		if (bHovering && m_pMenu->MainWindow->bMouse1released)
			IsGettingKey = true;
	}
	int alpha = m_pMenu->alpha;

	if (IsGettingKey || bHovering)
		g_pRender->FilledBoxOutlined(this->position.x, this->position.y, this->size.x, this->size.y, D3DCOLOR_ARGB(alpha, 35, 35, 35), BLACK(alpha));
	else
		g_pRender->GradientBoxOutlined(this->position.x, this->position.y, this->size.x, this->size.y, 1, D3DCOLOR_ARGB(alpha, 45, 45, 45), D3DCOLOR_ARGB(alpha, 35, 35, 35), BLACK(alpha), true);


		
	
		
	if (IsGettingKey)
	{
		KeyName = "<Press A Key>";
		for (int i = 0; i < 255; i++)
		{
			if (m_pMenu->MainWindow->ToggleButton(i))
			{
				if (i == VK_ESCAPE)
				{
					IsGettingKey = false;
					Key = -1;
					return;
				}

				Key = i;
				IsGettingKey = false;
				return;
			}
		}
	}
	else
	{
		if (Key >= 0)
		{
			KeyName = KeyStrings[Key];
		}
	}
	char buf[256];
	sprintf_s(buf, "%s: %s", name.c_str(), KeyName);
	g_pRender->Text(buf, position.x + size.x / 2, position.y, centered, g_pRender->Fonts.menu_control, true, WHITE(alpha), BLACK(alpha));

	//if (bHovering)
		//g_Draw.StringA(g_Draw.font_menu, true, m_pMenu->MainWindow->position.x + m_pMenu->MainWindow->size.x / 2, m_pMenu->MainWindow->position.y + m_pMenu->MainWindow->size.y + m_pMenu->MainWindow->dragYoffset + m_pMenu->MainWindow->dragYoffset / 2, 255, 255, 255, m_pMenu->alpha, "%s", this->caption.c_str());
		
	this->size.y +=  5;
	}