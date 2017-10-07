#include "sdk.h"
#include "controls.h"
char* KeyStrings1[254] = { "Not Bound", "Left Mouse", "Right Mouse", "Control+Break", "Middle Mouse", "Mouse 4", "Mouse 5",
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
void MTextbox::HandleKeys(int key)
{



	switch (key)
	{
	case VK_LSHIFT:
	case VK_RSHIFT:
	case VK_LCONTROL:
	case VK_RCONTROL:
	case VK_LEFT:
	case VK_RIGHT:
	case VK_CAPITAL:
	case VK_LBUTTON:
	case VK_RBUTTON:
		return;


	case VK_RETURN:
	case VK_ESCAPE:
		IsActive = false;
		return;

	case VK_SPACE:
		command.append(" ");
		return;

	case VK_BACK:
		if (command.length() > 0)
		{
			command.erase(command.length() - 1);
			return;
		}


	default:
		char* KeyName = "0";
		KeyName = KeyStrings1[key];
		//if (GetKeyNameText(key << 16, KeyName, 127))
		if (KeyName != "Backspace" && KeyName != "SHIFT" && KeyName != "CTRL" && KeyName != "ALT")
			command.append(KeyName);

		return;
	}
}
MTextbox::MTextbox(MBaseElement* parent, std::string name, std::string caption, MCategory* SaveCategory, std::string savename)
{
	this->parent = parent;
	this->name = name;
	this->size = Vector(parent->size.x - 20, 15);
	this->caption = caption;
	this->FileIdentifier = savename;
	this->FileControlType = UIControlTypes::UIC_Textbox;

	parent->AddChildControl(this);
	if (SaveCategory != nullptr)
		SaveCategory->SaveChildren.push_back(this);
}
void MTextbox::Draw()
{
	/*this->position = this->parent->position;
	this->position.x += 10;
	this->position.y += 10;

	this->size.x = parent->size.x - 20;

	int width = size.x - 1;

	for (int i = 0; i < this->parent->Children.size(); i++)
		if (this->parent->Children.at(i) != (MBaseElement*)this)
			this->position.y += this->parent->Children.at(i)->size.y; //boxes are 15 px big, that means 5 difference is natural

	bool bHover = m_pMenu->MainWindow->InBounds(position, size);

	g_Draw.FillRGBA(position.x, position.y, size.x, size.y, MidGrey.r(), MidGrey.g(), MidGrey.b(), MidGrey.a());
	if (bHover && m_pMenu->MainWindow->bMouse1released && !IsActive)
		IsActive = true;

	if (IsActive)
		g_Draw.FillRGBA(position.x, position.y, size.x, size.y, pWhite.r(), pWhite.g(), pWhite.b(), pWhite.a());
	else if (bHover)
		g_Draw.FillRGBA(position.x, position.y, size.x, size.y, LightGrey.r(), LightGrey.g(), LightGrey.b(), LightGrey.a());

	g_Draw.Border(position.x, position.y, size.x, size.y, 1, OutlineGrey.r(), OutlineGrey.g(), OutlineGrey.b(), OutlineGrey.a());

	if(command.empty() && !(bHover && m_pMenu->MainWindow->bMouse1pressed) && !IsActive)
		g_Draw.StringA(g_Draw.font_menu, true, position.x + size.x / 2, position.y, 255, 255, 255, 255, "%s", name.c_str());

	if (IsActive)
	{
		for (int i = 0; i < 255; i++)
		{
			if (m_pMenu->MainWindow->ToggleButton(i))
			{
				HandleKeys(i);
			}
		}
		if (m_pMenu->MainWindow->bMouse1released && !bHover)
			IsActive = false;
	}
	g_Draw.StringA(g_Draw.font_menu, true, position.x + size.x / 2, position.y, 255, 255, 255, 255, "%s", command.c_str());
	if (bHover)
		g_Draw.StringA(g_Draw.font_menu, true, m_pMenu->MainWindow->position.x + m_pMenu->MainWindow->size.x / 2, m_pMenu->MainWindow->position.y + m_pMenu->MainWindow->size.y + m_pMenu->MainWindow->dragYoffset + m_pMenu->MainWindow->dragYoffset / 2, 255, 255, 255, 255, "%s", this->caption.c_str());
*/
}