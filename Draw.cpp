#include "sdk.h"
#include "Draw.h"
#include "xor.h"
#include "resource.h"
CDraw g_Draw;
void CDraw::Init()
{

	if (g_pSurface == NULL)
	{
		MessageBox(0, XorStr("g_pSurface error"), XorStr("k"), 0);
		return;
	}
	font_esp = g_pSurface->SCreateFont();

	m_Font = g_pSurface->SCreateFont();
	g_pSurface->SetFontGlyphSet(m_Font, XorStr("Calibri"), 13, FW_DONTCARE, 0, 0, FONTFLAG_ANTIALIAS);

	g_pSurface->SetFontGlyphSet(font_esp, XorStr("Tahoma"), 12, 700, 0, 0, FONTFLAG_OUTLINE);

	font_GUI = g_pSurface->SCreateFont();
	g_pSurface->SetFontGlyphSet(font_GUI, XorStr("Copperplate Gothic Light"), 20, FW_DONTCARE, 0, 0, FONTFLAG_ANTIALIAS);

	font_menu = g_pSurface->SCreateFont();
	g_pSurface->SetFontGlyphSet(font_menu, XorStr("Verdana"), 12,FW_NORMAL, 0, 0, FONTFLAG_ANTIALIAS | FONTFLAG_DROPSHADOW | FONTFLAG_OUTLINE);


	font_build = g_pSurface->SCreateFont();
	g_pSurface->SetFontGlyphSet(font_build, XorStr("Verdana"), 28, FW_EXTRABOLD, 0, 0, FONTFLAG_ANTIALIAS);



	HRSRC res = FindResourceA(NULL, MAKEINTRESOURCE(IDR_FONT1), RT_FONT);
if (res) 
{
    HGLOBAL mem = LoadResource(GetModuleHandle(NULL), res);
    void *data = LockResource(mem);
    size_t len = SizeofResource(GetModuleHandle(NULL), res);

    DWORD nFonts;
    HANDLE m_fonthandle = AddFontMemResourceEx(
        data,       	// font resource
        len,       	// number of bytes in font resource 
        NULL,          	// Reserved. Must be 0.
        &nFonts      	// number of fonts installed
        );

    if(m_fonthandle==0)
    {
        //MessageBox(0, "Font add fails2", "Error", 0);
    }
}
else
{
	//MessageBox(0, std::to_string(GetLastError()).c_str(), "Error", 0);
}
auto __FONT = CreateFont(16, 0, 0, 0, 500, 0, 0, 0, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, "weapon_icons");

	font_weapon = g_pSurface->SCreateFont();
	g_pSurface->SetFontGlyphSet(font_weapon, "weapon_icons", 12, FW_DONTCARE, 0, 0, FONTFLAG_ANTIALIAS | FONTFLAG_OUTLINE);
}
void CDraw::String(int x, int y, int r, int g, int b, const wchar_t *pszText)
{
	if (pszText == NULL)
		return;

	g_pSurface->DrawSetTextPos(x, y);
	g_pSurface->DrawSetTextFont(m_Font);
	g_pSurface->DrawSetTextColor(r, g, b, 255);
	g_pSurface->DrawPrintText(pszText, wcslen(pszText));
}

std::wstring CDraw::stringToWide(const std::string& text)
{
	std::wstring wide(text.length(), L' ');
	std::copy(text.begin(), text.end(), wide.begin());

	return wide;
}

int CDraw::getWidht(const char* input, unsigned int font)
{
	int iWide = 0;
	int iTall = 0;
	int iBufSize = MultiByteToWideChar(CP_UTF8, 0x0, input, -1, NULL, 0);

	wchar_t* pszUnicode = new wchar_t[iBufSize];

	MultiByteToWideChar(CP_UTF8, 0x0, input, -1, pszUnicode, iBufSize);

	g_pSurface->GetTextSize(font, pszUnicode, iWide, iTall);

	delete[] pszUnicode;

	return iWide;
}


void CDraw::StringA(unsigned long Font, bool center, int x, int y, int r, int g, int b, int a, const char *input, ...)
{
	CHAR szBuffer[MAX_PATH];

	if (!input)
		return;

	vsprintf_s(szBuffer, input, (char*)&input + _INTSIZEOF(input));


	if (center)
		x -= getWidht(szBuffer, Font) / 2;

	g_pSurface->DrawSetTextColor(r, g, b, a);
	g_pSurface->DrawSetTextFont(Font);
	g_pSurface->DrawSetTextPos(x, y);

	std::wstring wide = stringToWide(std::string(szBuffer));
	g_pSurface->DrawPrintText(wide.c_str(), wide.length());
}


void CDraw::FillRGBA(int x, int y, int w, int h, int r, int g, int b, int a)
{
	g_pSurface->SetDrawColor(r, g, b, a);
	g_pSurface->DrawFilledRect(x, y, x + w, y + h);
}
void CDraw::FillRGBAOutlined(int x, int y, int w, int h, int r, int g, int b, int a)
{
	FillRGBA(x, y, w, h, r, g, b, a);
	Box(x, y, w, h, 0, 0, 0, a);
}
void CDraw::Box(int x, int y, int w, int h, int r, int g, int b, int a)
{
	g_pSurface->SetDrawColor(r, g, b, a);
	g_pSurface->DrawOutlinedRect(x, y, x + w, y + h);
}
/*void Render::DrawSphere(Vector& vecOrigin, unsigned int iRadius, Color col)
{
	auto& res = _spheres.find(iRadius);
	if (res != _spheres.end())
	{
		Vector vec2D;
		unsigned int iPoly = 0;
		unsigned int iSize = res->second.size();

		int* x0 = new int[24];
		int* y0 = new int[24];

		Interfaces::Surface->DrawSetColor(col);

		for (auto& point : res->second)
		{
			if (Render::WorldToScreen(point + vecOrigin, vec2D))
			{
				x0[iPoly] = vec2D.x;
				y0[iPoly] = vec2D.y;
				++iPoly;
			}

			if (((iPoly*//* + 1*//*) % 24) == 0 && iPoly > 0)
			{
				Interfaces::Surface->DrawPolyLine(x0, y0, iPoly);
				iPoly = 0;
			}
		}

		delete[] x0;
		delete[] y0;
	}
	else
	{
		unsigned int lati = 24;
		unsigned int loni = 48;
		for (unsigned int lat = 0; lat < lati; lat++)
		{
			unsigned int a1 = M_PI * (unsigned int)(lat + 1) / (lati + 1);
			unsigned int sin1 = sin(a1);
			unsigned int cos1 = cos(a1);

			for (unsigned int lon = 0; lon < loni; lon++)
			{
				unsigned int a2 = (2 * M_PI) * (unsigned int)(lon == loni ? 0 : lon) / loni;
				unsigned int sin2 = sin(a2);
				unsigned int cos2 = cos(a2);

				_spheres[iRadius].push_back(Vector(sin1 * cos2, cos1, sin1 * sin2) * iRadius);
			}
		}
		DrawSphere(vecOrigin, iRadius, col);
	}
}*/
void CDraw::LineRGBA(int x1, int y1, int x2, int y2, int r, int g, int b, int a)
{

	g_pSurface->SetDrawColor(r, g, b, a);
	g_pSurface->DrawLine(x1, y1, x2, y2);
}
void TextW(bool cent, unsigned int font, int x, int y, int r, int g, int b, int a, wchar_t *pszString)
{
	if (cent)
	{
		int wide, tall;
		g_pSurface->GetTextSize(font, pszString, wide, tall);
		x -= wide / 2;
		y -= tall / 2;
	}
	g_pSurface->DrawSetTextColor(r, g, b, a);
	g_pSurface->DrawSetTextFont(font);
	g_pSurface->DrawSetTextPos(x, y);
	g_pSurface->DrawPrintText(pszString, (int)wcslen(pszString));
}

void CDraw::Text(int x, int y, int r, int g, int b, int a, bool bCenter, unsigned int font, const char *fmt, ...)
{
	va_list va_alist;
	char szBuffer[1024] = { '\0' };

	va_start(va_alist, fmt);
	vsprintf_s(szBuffer, fmt, va_alist);
	va_end(va_alist);

	wchar_t *pszStringWide = reinterpret_cast< wchar_t* >(malloc((strlen(szBuffer) + 1) * sizeof(wchar_t)));

	size_t outSize;
	size_t size = strlen(szBuffer) + 1;
	mbstowcs_s(&outSize, pszStringWide, size, szBuffer, size-1);

	TextW(bCenter, font, x, y, r, g, b, a, pszStringWide);

	free(pszStringWide);
}
void CDraw::ColoredCircle(int x, int y, int radius, int r, int g, int b, int a)
{
	//g_pSurface->DrawColoredCircle(x, y, radius, r, g, b, a);
	g_pSurface->SetDrawColor(r, g, b, a);
	g_pSurface->DrawOutlinedCircle(x, y, radius, 32);
}
void CDraw::DrawRectRainbow(int x, int y, int width, int height, float flSpeed, float &flRainbow)
{
	Color colColor(0, 0, 0);

	flRainbow += flSpeed;
	if (flRainbow > 1.f) flRainbow = 0.f;

	for (int i = 0; i < width; i++)
	{
		float hue = (1.f / (float)width) * i;
		hue -= flRainbow;
		if (hue < 0.f) hue += 1.f;

		Color colRainbow = colColor.FromHSB(hue, 1.f, 1.f);
		FillRGBA(x + i, y, 1, height, colRainbow.r(), colRainbow.g(), colRainbow.b(), colRainbow.a());
	}
}
void CDraw::DrawTextureByteArray(Vector2D p0, Vector2D p1, Vector2D p2, Vector2D p3, const unsigned char* pRawRGBAData)
{
	static unsigned char buffer[4] = { 0,0,0,100 }; // R G B A (single pixel buffer layout.) 
	Vertex_t verts[4];
	static int Texture = g_pSurface->CreateNewTextureID(true);
	g_pSurface->DrawSetTextureRGBA(Texture, buffer/*pRawRGBAData*/, 1, 1);
	g_pSurface->SetDrawColor(255, 255, 255, 255);
	g_pSurface->DrawSetTexture(Texture);

	verts[0].Init(p0);
	verts[1].Init(p1);
	verts[2].Init(p2);
	verts[3].Init(p3);
	g_pSurface->DrawTexturedPolygon(4, verts);
}