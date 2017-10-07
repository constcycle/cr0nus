#include "sdk.h"
#include "Render.h"
#include "resource.h"
CRender* g_pRender = new CRender;

void CRender::Init(IDirect3DDevice9* pDevice)
{
	auto GenerateTexture = [](IDirect3DDevice9 *pD3Ddev, IDirect3DTexture9 **ppD3Dtex, DWORD colour32)
	{
		if (FAILED(pD3Ddev->CreateTexture(8, 8, 1, 0, D3DFMT_A4R4G4B4, D3DPOOL_MANAGED, ppD3Dtex, NULL)))
			return E_FAIL;

		WORD colour16 = ((WORD)((colour32 >> 28) & 0xF) << 12)
			| (WORD)(((colour32 >> 20) & 0xF) << 8)
			| (WORD)(((colour32 >> 12) & 0xF) << 4)
			| (WORD)(((colour32 >> 4) & 0xF) << 0);

		D3DLOCKED_RECT d3dlr;
		(*ppD3Dtex)->LockRect(0, &d3dlr, 0, 0);
		WORD *pDst16 = (WORD*)d3dlr.pBits;

		for (int xy = 0; xy < 8 * 8; xy++)
			*pDst16++ = colour16;

		(*ppD3Dtex)->UnlockRect(0);

		return S_OK;
	};

	this->device = pDevice;

	D3DXCreateLine(this->device, &this->line);
	D3DXCreateSprite(this->device, &this->sprite);

	GenerateTexture(pDevice, &this->white_tex, D3DCOLOR_ARGB(255, 255, 255, 255));

	/*HRSRC hResInst = FindResource(nullptr, MAKEINTRESOURCE(IDR_FONT1), "FONTDIR");
	HGLOBAL hGlobal = LoadResource(nullptr, hResInst);
	size_t __LEN = SizeofResource(nullptr, hResInst);
	DWORD dwFonts;
	HANDLE fonthandle = AddFontMemResourceEx(nullptr, __LEN, nullptr, &dwFonts);
	auto __FONT = CreateFont(16, 0, 0, 0, 500, 0, 0, 0, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, "weapon_icons");
	*/
	D3DXCreateFont(this->device, 12/*size*/, 0, /*(bold) ? FW_BOLD : */FW_BOLD, 1, /*(italic) ? 1 : */0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLEARTYPE_QUALITY, DEFAULT_PITCH,"Tahoma", &this->Fonts.esp);

	//D3DXCreateFont(this->device, 12/*size*/, 0, /*(bold) ? FW_BOLD : */FW_DONTCARE, 1, /*(italic) ? 1 : */0, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLEARTYPE_QUALITY, DEFAULT_PITCH, "weapon_icons", &this->Fonts.weapon_icon);

	D3DXCreateFontA(this->device, this->menu_control_size/*size*/, 0, 400, 1, /*(italic) ? 1 : */0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLEARTYPE_QUALITY, DEFAULT_PITCH, "Verdana", &this->Fonts.menu_control);

	return;
}

void CRender::Reset()
{
	D3DVIEWPORT9 screen;
	this->device->GetViewport(&screen);
	
	Screen.Width = screen.Width;
	Screen.Height = screen.Height;
	Screen.x_center = Screen.Width / 2;
	Screen.y_center = Screen.Height / 2;
}


int CRender::StringWidth(ID3DXFont* font, char* string) {
	RECT pRect = RECT();
	font->DrawText(NULL, string, strlen(string), &pRect, DT_CALCRECT, D3DCOLOR_RGBA(0, 0, 0, 0));
	return pRect.right - pRect.left;
}
void CRender::Line(int x, int y, int x2, int y2, D3DCOLOR color) 
{
	vertex pVertex[2] = { { x, y, 0.0f, 1.0f, color },{ x2, y2, 0.0f, 1.0f, color } };
	
	this->device->SetFVF(D3DFVF_XYZRHW | D3DFVF_DIFFUSE);
	this->device->SetTexture(0, nullptr);
	this->device->SetRenderState(D3DRS_LIGHTING, FALSE);
	this->device->SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE);
	this->device->SetRenderState(D3DRS_FOGENABLE, FALSE);
	this->device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	this->device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	this->device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	this->device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	this->device->SetRenderState(D3DRS_LIGHTING, FALSE);
	this->device->SetRenderState(D3DRS_STENCILENABLE, FALSE);

	this->device->DrawPrimitiveUP(D3DPT_LINELIST, 1, &pVertex, sizeof(vertex));

	/*const DWORD D3DFVF_TL = D3DFVF_XYZRHW | D3DFVF_DIFFUSE;

	DWORD dwTmpFVF;

	this->device->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
	this->device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	this->device->SetFVF(D3DFVF_TL);
	this->device->GetFVF(&dwTmpFVF);
	this->device->SetTexture(0, NULL);
	this->device->SetPixelShader(0); this->device->SetFVF(D3DFVF_XYZRHW | D3DFVF_DIFFUSE);
	

	this->device->DrawPrimitiveUP(D3DPT_LINELIST, 1, &pVertex, sizeof(vertex));

	if (dwTmpFVF)
		this->device->SetFVF(dwTmpFVF);*/


	/*auto DistBetweenTwoPoints = [](int sX, int sY, int eX, int eY) -> float
	{
		int dX = sX - eX;
		int dY = sY - eY;

		return sqrt(dX*dX + dY*dY);
	};

	float flLength = DistBetweenTwoPoints(x, y, x2, y2);
	float flAngle = atan((float)(y2 - y) / (x2 - x));


	D3DXMATRIX		d3dMatrix, d3dOldMatrix;
	RECT			rct;

	D3DXVECTOR2		vCenter(x, y);
	D3DXVECTOR3		vPos(x, y, 0);

	rct.left = 0;
	rct.top = 0;
	rct.right = flLength;
	rct.bottom = 1;

	D3DXMatrixTransformation2D(&d3dMatrix, NULL, NULL, NULL, &vCenter, flAngle, NULL);

	this->sprite->GetTransform(&d3dOldMatrix);

	this->sprite->SetTransform(&d3dMatrix);
	this->sprite->Draw(this->white_tex, &rct, NULL, &vPos, color);

	this->sprite->SetTransform(&d3dOldMatrix);
	*/
}
void CRender::FilledBox(int x, int y, int width, int height, D3DCOLOR color) 
{
	vertex pVertex[4] = { { x, y + height, 0.0f, 1.0f, color },{ x, y, 0.0f, 1.0f, color },{ x + width, y + height, 0.0f, 1.0f, color },{ x + width, y, 0.0f, 1.0f, color } };
	this->device->SetFVF(D3DFVF_XYZRHW | D3DFVF_DIFFUSE);
	this->device->SetTexture(0, nullptr);
	this->device->SetRenderState(D3DRS_LIGHTING, FALSE);
	this->device->SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE);
	this->device->SetRenderState(D3DRS_FOGENABLE, FALSE);
	this->device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	this->device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	this->device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	this->device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	this->device->SetRenderState(D3DRS_LIGHTING, FALSE);
	this->device->SetRenderState(D3DRS_STENCILENABLE, FALSE);
	this->device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, pVertex, sizeof(vertex));
	/*vertex pVertex[4] = { { x, y + height, 0.0f, 1.0f, color },{ x, y, 0.0f, 1.0f, color },{ x + width, y + height, 0.0f, 1.0f, color },{ x + width, y, 0.0f, 1.0f, color } };
	const DWORD D3DFVF_TL = D3DFVF_XYZRHW | D3DFVF_DIFFUSE;

	DWORD dwTmpFVF;

	this->device->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
	this->device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	this->device->SetFVF(D3DFVF_TL);
	this->device->GetFVF(&dwTmpFVF);
	this->device->SetTexture(0, NULL);
	
	this->device->SetPixelShader(0);

	this->device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, pVertex, sizeof(vertex));

	if (dwTmpFVF)
		this->device->SetFVF(dwTmpFVF);
		*/
	/*RECT rct;
	rct.left = 0;
	rct.top = 0;
	rct.right = width;
	rct.bottom = height;

	D3DXVECTOR3 pos;
	pos.x = x;
	pos.y = y;
	pos.z = 0;

	this->sprite->Draw(this->white_tex, &rct, NULL, &pos, color);*/
}
void CRender::FilledBoxOutlined(int x, int y, int width, int height, D3DCOLOR color, D3DCOLOR outlinecolor, int thickness) 
{
	
	this->FilledBox(x, y, width, height, color);
	this->BorderedBox(x, y, width, height, outlinecolor);
}
void CRender::BorderedBox(int x, int y, int width, int height, D3DCOLOR color, int thickness) 
{
	this->FilledBox(x, y, width, thickness, color);
	this->FilledBox(x, y, thickness, height, color);
	this->FilledBox(x + width - thickness, y, thickness, height, color);
	this->FilledBox(x, y + height - thickness, width, thickness, color);
	
}
void CRender::BorderedBoxOutlined(int x, int y, int width, int height,  D3DCOLOR color, D3DCOLOR outlinecolor, int thickness) {
	this->BorderedBox(x, y, width, height, outlinecolor, thickness);
	this->BorderedBox(x + thickness, y + thickness, width - (thickness * 2), height - (thickness * 2), color, thickness);
	this->BorderedBox(x + (thickness * 2), y + (thickness * 2), width - (thickness * 4), height - (thickness * 4),  outlinecolor, thickness);
}
void CRender::GradientBox(int x, int y, int width, int height, D3DCOLOR color, D3DCOLOR color2, bool vertical) {
	vertex pVertex[4] = { { x, y, 0.0f, 1.0f, color },{ x + width, y, 0.0f, 1.0f, vertical ? color : color2 },{ x, y + height, 0.0f, 1.0f, vertical ? color2 : color },{ x + width, y + height, 0.0f, 1.0f, color2 } };
	this->device->SetFVF(D3DFVF_XYZRHW | D3DFVF_DIFFUSE);
	this->device->SetTexture(0, nullptr);
	this->device->SetRenderState(D3DRS_LIGHTING, FALSE);
	this->device->SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE);
	this->device->SetRenderState(D3DRS_FOGENABLE, FALSE);
	this->device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	this->device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	this->device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	this->device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	this->device->SetRenderState(D3DRS_LIGHTING, FALSE);
	this->device->SetRenderState(D3DRS_STENCILENABLE, FALSE);
	this->device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, pVertex, sizeof(vertex));
}
void CRender::GradientBoxOutlined(int x, int y, int width, int height, int thickness, D3DCOLOR color, D3DCOLOR color2, D3DCOLOR outlinecolor, bool vertical) {
	
	this->GradientBox(x , y, width, height, color, color2, vertical);
	this->BorderedBox(x, y, width, height, outlinecolor, thickness);
}
void CRender::Circle(int x, int y, int radius, int points, D3DCOLOR color) 
{
	vertex* pVertex = new vertex[points + 1];
	for (int i = 0; i <= points; i++) pVertex[i] = { x + radius * cos(D3DX_PI * (i / (points / 2.0f))), y - radius * sin(D3DX_PI * (i / (points / 2.0f))), 0.0f, 1.0f, color };
	this->device->SetFVF(D3DFVF_XYZRHW | D3DFVF_DIFFUSE);
	this->device->DrawPrimitiveUP(D3DPT_LINESTRIP, points, pVertex, sizeof(vertex));
	delete[] pVertex;
}
void CRender::FilledCircle(int x, int y, int radius, int points, D3DCOLOR color) {
	vertex* pVertex = new vertex[points + 1];
	for (int i = 0; i <= points; i++) pVertex[i] = { x + radius * cos(D3DX_PI * (i / (points / 2.0f))), y + radius * sin(D3DX_PI * (i / (points / 2.0f))), 0.0f, 1.0f, color };
	this->device->SetFVF(D3DFVF_XYZRHW | D3DFVF_DIFFUSE);
	this->device->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, points, pVertex, sizeof(vertex));
	delete[] pVertex;
}

void CRender::Text(char *text, float x, float y, int orientation, LPD3DXFONT pFont, bool bordered, DWORD color, DWORD bcolor)
{
	
	RECT rect;

	switch (orientation)
	{
	case lefted:
		if (bordered)
		{
			SetRect(&rect, x - 1, y, x - 1, y);
			pFont->DrawTextA(NULL, text, -1, &rect, DT_LEFT | DT_NOCLIP, bcolor);
			SetRect(&rect, x + 1, y, x + 1, y);
			pFont->DrawTextA(NULL, text, -1, &rect, DT_LEFT | DT_NOCLIP, bcolor);
			SetRect(&rect, x, y - 1, x, y - 1);
			pFont->DrawTextA(NULL, text, -1, &rect, DT_LEFT | DT_NOCLIP, bcolor);
			SetRect(&rect, x, y + 1, x, y + 1);
			pFont->DrawTextA(NULL, text, -1, &rect, DT_LEFT | DT_NOCLIP, bcolor);
		}
		SetRect(&rect, x, y, x, y);
		pFont->DrawTextA(NULL, text, -1, &rect, DT_LEFT | DT_NOCLIP, color);
		break;
	case centered:
		if (bordered)
		{
			SetRect(&rect, x - 1, y, x - 1, y);
			pFont->DrawTextA(NULL, text, -1, &rect, DT_CENTER | DT_NOCLIP, bcolor);
			SetRect(&rect, x + 1, y, x + 1, y);
			pFont->DrawTextA(NULL, text, -1, &rect, DT_CENTER | DT_NOCLIP, bcolor);
			SetRect(&rect, x, y - 1, x, y - 1);
			pFont->DrawTextA(NULL, text, -1, &rect, DT_CENTER | DT_NOCLIP, bcolor);
			SetRect(&rect, x, y + 1, x, y + 1);
			pFont->DrawTextA(NULL, text, -1, &rect, DT_CENTER | DT_NOCLIP, bcolor);
		}
		SetRect(&rect, x, y, x, y);
		pFont->DrawTextA(NULL, text, -1, &rect, DT_CENTER | DT_NOCLIP, color);
		break;
	case righted:
		if (bordered)
		{
			SetRect(&rect, x - 1, y, x - 1, y);
			pFont->DrawTextA(NULL, text, -1, &rect, DT_RIGHT | DT_NOCLIP, bcolor);
			SetRect(&rect, x + 1, y, x + 1, y);
			pFont->DrawTextA(NULL, text, -1, &rect, DT_RIGHT | DT_NOCLIP, bcolor);
			SetRect(&rect, x, y - 1, x, y - 1);
			pFont->DrawTextA(NULL, text, -1, &rect, DT_RIGHT | DT_NOCLIP, bcolor);
			SetRect(&rect, x, y + 1, x, y + 1);
			pFont->DrawTextA(NULL, text, -1, &rect, DT_RIGHT | DT_NOCLIP, bcolor);
		}
		SetRect(&rect, x, y, x, y);
		pFont->DrawTextA(NULL, text, -1, &rect, DT_RIGHT | DT_NOCLIP, color);
		break;
	}
}
void CRender::String(float x, float y, int orientation, LPD3DXFONT pFont, bool bordered, DWORD color, const char *input, ...)
{
	CHAR szBuffer[MAX_PATH];

	if (!input)
		return;


	DWORD bcolor = BLACK(255);

	vsprintf_s(szBuffer, input, (char*)&input + _INTSIZEOF(input));

	RECT rect;

	switch (orientation)
	{
	case lefted:
		if (bordered)
		{
			SetRect(&rect, x - 1, y, x - 1, y);
			pFont->DrawTextA(NULL, szBuffer, -1, &rect, DT_LEFT | DT_NOCLIP, bcolor);
			SetRect(&rect, x + 1, y, x + 1, y);
			pFont->DrawTextA(NULL, szBuffer, -1, &rect, DT_LEFT | DT_NOCLIP, bcolor);
			SetRect(&rect, x, y - 1, x, y - 1);
			pFont->DrawTextA(NULL, szBuffer, -1, &rect, DT_LEFT | DT_NOCLIP, bcolor);
			SetRect(&rect, x, y + 1, x, y + 1);
			pFont->DrawTextA(NULL, szBuffer, -1, &rect, DT_LEFT | DT_NOCLIP, bcolor);
		}
		SetRect(&rect, x, y, x, y);
		pFont->DrawTextA(NULL, szBuffer, -1, &rect, DT_LEFT | DT_NOCLIP, color);
		break;
	case centered:
		if (bordered)
		{
			SetRect(&rect, x - 1, y, x - 1, y);
			pFont->DrawTextA(NULL, szBuffer, -1, &rect, DT_CENTER | DT_NOCLIP, bcolor);
			SetRect(&rect, x + 1, y, x + 1, y);
			pFont->DrawTextA(NULL, szBuffer, -1, &rect, DT_CENTER | DT_NOCLIP, bcolor);
			SetRect(&rect, x, y - 1, x, y - 1);
			pFont->DrawTextA(NULL, szBuffer, -1, &rect, DT_CENTER | DT_NOCLIP, bcolor);
			SetRect(&rect, x, y + 1, x, y + 1);
			pFont->DrawTextA(NULL, szBuffer, -1, &rect, DT_CENTER | DT_NOCLIP, bcolor);
		}
		SetRect(&rect, x, y, x, y);
		pFont->DrawTextA(NULL, szBuffer, -1, &rect, DT_CENTER | DT_NOCLIP, color);
		break;
	case righted:
		if (bordered)
		{
			SetRect(&rect, x - 1, y, x - 1, y);
			pFont->DrawTextA(NULL, szBuffer, -1, &rect, DT_RIGHT | DT_NOCLIP, bcolor);
			SetRect(&rect, x + 1, y, x + 1, y);
			pFont->DrawTextA(NULL, szBuffer, -1, &rect, DT_RIGHT | DT_NOCLIP, bcolor);
			SetRect(&rect, x, y - 1, x, y - 1);
			pFont->DrawTextA(NULL, szBuffer, -1, &rect, DT_RIGHT | DT_NOCLIP, bcolor);
			SetRect(&rect, x, y + 1, x, y + 1);
			pFont->DrawTextA(NULL, szBuffer, -1, &rect, DT_RIGHT | DT_NOCLIP, bcolor);
		}
		SetRect(&rect, x, y, x, y);
		pFont->DrawTextA(NULL, szBuffer, -1, &rect, DT_RIGHT | DT_NOCLIP, color);
		break;
	}
}



void CRender::Message(char *text, float x, float y, LPD3DXFONT pFont, int orientation)
{
	RECT rect = { x, y, x, y };

	switch (orientation)
	{
	case lefted:
		pFont->DrawTextA(NULL, text, -1, &rect, DT_CALCRECT | DT_LEFT, BLACK(255));

		this->BorderedBox(x - 5, rect.top - 5, rect.right - x + 10, rect.bottom - rect.top + 10, 5, SKYBLUE(255));

		SetRect(&rect, x, y, x, y);
		pFont->DrawTextA(NULL, text, -1, &rect, DT_LEFT | DT_NOCLIP, ORANGE(255));
		break;
	case centered:
		pFont->DrawTextA(NULL, text, -1, &rect, DT_CALCRECT | DT_CENTER, BLACK(255));

		this->BorderedBox(rect.left - 5, rect.top - 5, rect.right - rect.left + 10, rect.bottom - rect.top + 10, 5 ,SKYBLUE(255));

		SetRect(&rect, x, y, x, y);
		pFont->DrawTextA(NULL, text, -1, &rect, DT_CENTER | DT_NOCLIP, ORANGE(255));
		break;
	case righted:
		pFont->DrawTextA(NULL, text, -1, &rect, DT_CALCRECT | DT_RIGHT, BLACK(255));

		this->BorderedBox(rect.left - 5, rect.top - 5, rect.right - rect.left + 10, rect.bottom - rect.top + 10, 5, SKYBLUE(255));

		SetRect(&rect, x, y, x, y);
		pFont->DrawTextA(NULL, text, -1, &rect, DT_RIGHT | DT_NOCLIP, ORANGE(255));
		break;
	}
}


void CRender::Sprite(LPDIRECT3DTEXTURE9 tex, float x, float y, float resolution_x, float resolution_y, float scale_x, float scale_y, float rotation, DWORD color)
{
	float screen_pos_x = x;
	float screen_pos_y = y;

	
	// Screen position of the sprite
	D3DXVECTOR2 trans = D3DXVECTOR2(screen_pos_x, screen_pos_y);

	// Build our matrix to rotate, scale and position our sprite
	D3DXMATRIX mat;

	//addition to have same icons
	scale_x = 64 / resolution_x;
	scale_y = 64 / resolution_y;

	// Texture being used is by native resolution
	//D3DXVECTOR2 spriteCentre = D3DXVECTOR2(resolution_x / 2, resolution_y / 2);


	// Texture being used is 64x64:
	D3DXVECTOR2 spriteCentre = D3DXVECTOR2(64/ 2, 64/ 2);


	D3DXVECTOR2 scaling(scale_x, scale_y);

	// out, scaling centre, scaling rotation, scaling, rotation centre, rotation, translation
	D3DXMatrixTransformation2D(&mat, NULL, 0.0, &scaling, &spriteCentre, rotation, &trans);

	//this->device->SetRenderState(D3DRS_ZENABLE, false);
	this->device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	this->device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	this->device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	this->device->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
	this->device->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	this->device->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
	this->device->SetFVF(D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1);
	this->device->SetPixelShader(NULL);
	this->sprite->Begin(NULL);
	this->sprite->SetTransform(&mat); // Tell the sprite about the matrix
	this->sprite->Draw(tex, NULL, NULL, NULL, color);
	//this->sprite->Draw(tex, NULL, NULL, &D3DXVECTOR3(x, y, 0), 0xFFFFFFFF);
	this->sprite->End();
}

int CRender::FrameRate() 
{
	static int iFps, iLastFps;
	static float flLastTickCount, flTickCount;
	flTickCount = clock() * 0.001f;
	iFps++;
	if ((flTickCount - flLastTickCount) >= 1.0f) {
		flLastTickCount = flTickCount;
		iLastFps = iFps;
		iFps = 0;
	}
	return iLastFps;
}
