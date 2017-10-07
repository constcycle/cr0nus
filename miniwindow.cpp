#include "sdk.h"
#include "controls.h"
#include "Render.h"
#include "Menu.h"
MMiniWindow::MMiniWindow(std::string name, Vector pos, Vector size, MBaseElement* parent, bool render_if_no_menu)
{
	this->position = pos;
	this->size = size;
	this->name = name;
	//this->parent = parent;
	//this->parent->AddChildControl(this);
	bRender = render_if_no_menu;
}
void MMiniWindow::Draw()
{


	int alpha = bRender ? 255 : m_pMenu->alpha;

	if (alpha == 0)
		return;

	int dragyoffset = m_pMenu->MainWindow->dragYoffset/2;

	if ((m_pMenu->MainWindow->InBounds(Vector(this->position.x, this->position.y), Vector(this->size.x, dragyoffset)) && m_pMenu->MainWindow->bMouse1pressed))
		this->bDragging = true;
	if (this->bDragging)
	{
		if (m_pMenu->MainWindow->bMouse1pressed)
		{
			if (iXDif == -1 || iYDif == -1)
			{
				iXDif = m_pMenu->MainWindow->Cur.x - this->position.x;
				iYDif = m_pMenu->MainWindow->Cur.y - this->position.y;
			}
			this->position.x += m_pMenu->MainWindow->Cur.x - (iXDif + this->position.x);
			this->position.y += m_pMenu->MainWindow->Cur.y - (iYDif + this->position.y);

		
		}
		else
			this->bDragging = false;
	}
	else
	{
		iXDif = -1;
		iYDif = -1;
	}

	


		/*dragbar*/
	g_pRender->FilledBox(this->position.x, this->position.y, this->size.x, dragyoffset, D3DCOLOR_ARGB(alpha, 12, 12, 12));
	/*background*/
	g_pRender->FilledBox(this->position.x, this->position.y + dragyoffset, this->size.x, this->size.y - dragyoffset, D3DCOLOR_ARGB(alpha, 17, 17, 17));
	/*outline*/
	g_pRender->BorderedBox(this->position.x - 1, this->position.y - 1, this->size.x + 2, this->size.y + 2, BLACK(alpha));
	g_pRender->BorderedBox(this->position.x - 2, this->position.y - 2, this->size.x + 4, this->size.y + 4, D3DCOLOR_ARGB(alpha, 41, 42, 45));
	g_pRender->BorderedBox(this->position.x - 3, this->position.y - 3, this->size.x + 6, this->size.y + 6, D3DCOLOR_ARGB(alpha, 41, 42, 45));
	g_pRender->BorderedBox(this->position.x - 4, this->position.y - 4, this->size.x + 8, this->size.y + 8, D3DCOLOR_ARGB(alpha, 41, 42, 45));
	g_pRender->BorderedBox(this->position.x - 5, this->position.y - 5, this->size.x + 10, this->size.y + 10, D3DCOLOR_ARGB(alpha, 41, 42, 45));
	g_pRender->BorderedBox(this->position.x - 6, this->position.y - 6, this->size.x + 12, this->size.y + 12, D3DCOLOR_ARGB(alpha, 41, 42, 45));
	g_pRender->BorderedBox(this->position.x - 7, this->position.y - 7, this->size.x + 14, this->size.y + 14, BLACK(alpha));

	g_pRender->String(this->position.x + this->size.x / 2, this->position.y + (dragyoffset / 2) - (g_pRender->menu_control_size / 2), centered, g_pRender->Fonts.menu_control, true, WHITE(alpha), "%s", this->name.c_str());


	

	for (int i = 0; i < Children.size(); i++)
		Children[i]->Draw();
}