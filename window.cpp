#include "sdk.h"
#include "controls.h"
#include "Menu.h"
#include "Render.h"

#pragma region keyshit
bool keys[256];
bool oldKeys[256];

#pragma endregion
#pragma region input
bool MWindow::ToggleButton(int code)
{
	static int buttonPressedTick = 0;
	if (GetAsyncKeyState(code) && (GetTickCount64() - buttonPressedTick) > 150)
	{
		buttonPressedTick = GetTickCount64();
		return true;
	}
	return false;
}
bool MWindow::GetKeyPress(unsigned int key)
{
	if (keys[key] == true && oldKeys[key] == false)
		return true;
	else
		return false;
}
void MWindow::ClickHandler()
{
	if ((GetAsyncKeyState(VK_LBUTTON)))
	{
		bMouse1pressed = true;
	}
	else if (!(GetAsyncKeyState(VK_LBUTTON)))
	{
		if (bMouse1pressed)
			bMouse1released = true;
		else
		{
			bMouse1released = false;
		}
		bMouse1pressed = false;
	}

	if ((GetAsyncKeyState(VK_RBUTTON)))
	{
		bMouse2pressed = true;
	}
	else if (!(GetAsyncKeyState(VK_RBUTTON)))
	{
		if (bMouse2pressed)
			bMouse2released = true;
		else
		{
			bMouse2released = false;
		}
		bMouse2pressed = false;
	}
}
bool MWindow::Clicked(Vector position, Vector bounds)
{
	
	return (Cur.x > position.x && Cur.y > position.y &&
		Cur.x < position.x + bounds.x && Cur.y < position.y + bounds.y &&
		bMouse1released);//ToggleButton(VK_LBUTTON));
}

bool MWindow::InBounds(Vector position, Vector bounds)
{
	
	return (Cur.x > position.x && Cur.y > position.y &&
		Cur.x < position.x + bounds.x && Cur.y < position.y + bounds.y);
}
#pragma endregion
MWindow::MWindow(std::string name, Vector pos, Vector size, bool IsMainWindow)
{
	this->name = name;
	this->position = pos;
	this->size = size;
	this->visible = false;
	this->bDragging = false;
	this->IsMainWindow = IsMainWindow;
	categoryXoffset = 100;// this->size.x / 5.3333333f;
	dragYoffset = 70;// this->size.y / 10;
	//init move
	this->moved_in_that_frame = true;
}



void MWindow::Draw()
{
	static bool risetotop = false;
	static bool risetobottom = false;
	if (ToggleButton(VK_INSERT))
	{
		this->visible = !this->visible;
		char buf[32];
		sprintf_s(buf, "cl_mouseenable %i", !this->visible);
		g_pEngine->ClientCmd(buf);
		if (this->visible)
		{
			//g_pInputSystem->EnableInput(false);
			risetobottom = false;
			risetotop = true;
		}
		else
		{
			//g_pInputSystem->ResetInputState();
			//g_pInputSystem->EnableInput(true);
			risetobottom = true;
			risetotop = false;
		}
		
	}
	if (m_pMenu->alpha >= 255)
		risetotop = false;
	else if (m_pMenu->alpha <= 0)
		risetobottom = false;

	if (risetobottom)
		m_pMenu->alpha -= 3;

	if (risetotop)
		m_pMenu->alpha += 3;

	if (!this->visible && m_pMenu->alpha == 0)
		return;


	//Surface->LockCursor();

	ClickHandler();
	HWND Window = FindWindow(NULL, "Counter-Strike: Global Offensive");
	GetCursorPos(&Cur);
	ScreenToClient(Window, &Cur);

	
	std::copy(keys, keys + 255, oldKeys);
	for (int x = 0; x < 255; x++)
	{
		//oldKeys[x] = oldKeys[x] & keys[x];
		keys[x] = (GetAsyncKeyState(x));
	}

	static int iXDif = 0;
	static int iYDif = 0;


	/*Menu dragging*/
	/*Adjusted for themes, just delete the addition with categoryXoFsset*/
	if ((InBounds(Vector(this->position.x, this->position.y), Vector(this->size.x, dragYoffset)) && bMouse1pressed))
		this->bDragging = true;
	if (this->bDragging)
	{
		if (bMouse1pressed)
		{
			if (iXDif == -1 || iYDif == -1)
			{
				iXDif = Cur.x - this->position.x;
				iYDif = Cur.y - this->position.y;
			}
			this->position.x += Cur.x - (iXDif + this->position.x);
			this->position.y += Cur.y - (iYDif + this->position.y);

			this->moved_in_that_frame = true;
		}
		else
			this->bDragging = false;
	}
	else
	{
		iXDif = -1;
		iYDif = -1;
	}

	
		

	int alpha = m_pMenu->alpha;

	/*dragbar*/
	g_pRender->FilledBox(this->position.x, this->position.y, this->size.x, dragYoffset, D3DCOLOR_ARGB(alpha, 12, 12, 12));
	/*background*/
	g_pRender->FilledBox(this->position.x, this->position.y +  dragYoffset, this->size.x, this->size.y - dragYoffset ,D3DCOLOR_ARGB(alpha, 17, 17, 17));
	/*outline*/
	g_pRender->BorderedBox(this->position.x - 1, this->position.y - 1, this->size.x + 2, this->size.y + 2, BLACK(alpha));
	g_pRender->BorderedBox(this->position.x - 2, this->position.y - 2, this->size.x + 4, this->size.y + 4, D3DCOLOR_ARGB(alpha, 41, 42, 45));
	g_pRender->BorderedBox(this->position.x - 3, this->position.y - 3, this->size.x + 6, this->size.y + 6, D3DCOLOR_ARGB(alpha, 41, 42, 45));
	g_pRender->BorderedBox(this->position.x - 4, this->position.y - 4, this->size.x + 8, this->size.y + 8, D3DCOLOR_ARGB(alpha, 41, 42, 45));
	g_pRender->BorderedBox(this->position.x - 5, this->position.y - 5, this->size.x + 10, this->size.y + 10, D3DCOLOR_ARGB(alpha, 41, 42, 45));
	g_pRender->BorderedBox(this->position.x - 6, this->position.y - 6, this->size.x + 12, this->size.y + 12, D3DCOLOR_ARGB(alpha, 41, 42, 45));
	g_pRender->BorderedBox(this->position.x - 7, this->position.y - 7, this->size.x + 14, this->size.y + 14, BLACK(alpha));
	
	//g_Draw.StringA(g_Draw.font_GUI, true, this->position.x + categoryXoffset - 10 + (this->size.x - categoryXoffset) / 2, this->position.y + 15, 255, 255, 255, m_pMenu->alpha, "%s", this->name.c_str());
	
	for (int i = 0; i < Children.size(); i++)
		Children[i]->Draw();

	//m_pMenu->SkinChanger->Draw();

	

}

void MWindow::DrawMouseCursor(POINT Cur)
{
	/*int x = Cur.x;
	int y = Cur.y;
	g_Draw.FillRGBA(x + 1, y, 1, 17, 3, 6, 26, 255);
	for (int i = 0; i < 11; i++)
		g_Draw.FillRGBA(x + 2 + i, y + 1 + i, 1, 1, 3, 6, 26, 255);
	g_Draw.FillRGBA(x + 8, y + 12, 5, 1, 3, 6, 26, 255);
	g_Draw.FillRGBA(x + 8, y + 13, 1, 1, 3, 6, 26, 255);
	g_Draw.FillRGBA(x + 9, y + 14, 1, 2, 3, 6, 26, 255);
	g_Draw.FillRGBA(x + 10, y + 16, 1, 2, 3, 6, 26, 255);
	g_Draw.FillRGBA(x + 8, y + 18, 2, 1, 3, 6, 26, 255);
	g_Draw.FillRGBA(x + 7, y + 16, 1, 2, 3, 6, 26, 255);
	g_Draw.FillRGBA(x + 6, y + 14, 1, 2, 3, 6, 26, 255);
	g_Draw.FillRGBA(x + 5, y + 13, 1, 1, 3, 6, 26, 255);
	g_Draw.FillRGBA(x + 4, y + 14, 1, 1, 3, 6, 26, 255);
	g_Draw.FillRGBA(x + 3, y + 15, 1, 1, 3, 6, 26, 255);
	g_Draw.FillRGBA(x + 2, y + 16, 1, 1, 3, 6, 26, 255);
	for (int i = 0; i < 4; i++)
		g_Draw.FillRGBA(x + 2 + i, y + 2 + i, 1, 14 - (i * 2), 255 - (i * 4), 255 - (i * 4), 255 - (i * 4), 255);
	g_Draw.FillRGBA(x + 6, y + 6, 1, 8, 235, 235, 235, 255);
	g_Draw.FillRGBA(x + 7, y + 7, 1, 9, 231, 231, 231, 255);
	for (int i = 0; i < 4; i++)
		g_Draw.FillRGBA(x + 8 + i, y + 8 + i, 1, 4 - i, 227 - (i * 4), 227 - (i * 4), 227 - (i * 4), 255);
	g_Draw.FillRGBA(x + 8, y + 14, 1, 4, 207, 207, 207, 255);
	g_Draw.FillRGBA(x + 9, y + 16, 1, 2, 203, 203, 203, 255);*/
}