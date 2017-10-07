#include "sdk.h"
#include "controls.h"
MButton::MButton(MBaseElement* parent, Vector position, Vector size, std::string name, std::string caption,ButtonCallback_t Function)
{
	this->name = name;
	this->caption = caption;
	this->pos = position;
	this->size = size;
	this->parent = parent;
	this->CallBack = Function;
}
void MButton::Draw()
{
	/*this->position = parent->position + this->pos;
	bool bHover = m_pMenu->MainWindow->InBounds(position, size);

	g_Draw.Border(position.x, position.y, size.x, size.y, 1, 0, 0, 0, 255);

	if (m_pMenu->MainWindow->bMouse1pressed && bHover)
		g_Draw.FillRGBA(position.x + 1, position.y + 1, size.x - 1, size.y - 1, 0, 158, 235, 255);
	else if (bHover)
		g_Draw.FillRGBA(position.x + 1, position.y + 1, size.x - 1, size.y - 1, ABlue.r(), ABlue.g(), ABlue.b(), ABlue.a());

	g_Draw.StringA(g_Draw.font_menu, true, position.x + size.x / 2, position.y, m_pMenu->MainWindow->ActiveFontColor.r(), m_pMenu->MainWindow->ActiveFontColor.g(), m_pMenu->MainWindow->ActiveFontColor.b(), m_pMenu->MainWindow->ActiveFontColor.a(), "%s", name.c_str());

	if (bHover)
		g_Draw.StringA(g_Draw.font_menu, true, m_pMenu->MainWindow->position.x + m_pMenu->MainWindow->size.x / 2, m_pMenu->MainWindow->position.y + m_pMenu->MainWindow->size.y + m_pMenu->MainWindow->dragYoffset + m_pMenu->MainWindow->dragYoffset / 2, m_pMenu->MainWindow->ActiveFontColor.r(), m_pMenu->MainWindow->ActiveFontColor.g(), m_pMenu->MainWindow->ActiveFontColor.b(), m_pMenu->MainWindow->ActiveFontColor.a(), "%s", this->caption.c_str());

	if (bHover && m_pMenu->MainWindow->bMouse1released)
		if (CallBack)
			CallBack();*/
}