#pragma once
class CDraw
{
public: 
	void Init();
	int getWidht(const char* input, unsigned int font);
	std::wstring stringToWide(const std::string& text);
	void String(int x, int y, int r, int g, int b, const wchar_t *pszText);
	void StringA(unsigned long Font, bool center, int x, int y, int r, int g, int b, int a, const char *input, ...);
	void FillRGBA(int x, int y, int w, int h, int r, int g, int b, int a);
	void FillRGBAOutlined(int x, int y, int w, int h, int r, int g, int b, int a);
	void Box(int x, int y, int w, int h, int r, int g, int b, int a);
	void LineRGBA(int x1, int y1, int x2, int y2, int r, int g, int b, int a);
	void Text(int x, int y, int r, int g, int b, int a, bool bCenter, unsigned int font, const char *fmt, ...);
	void ColoredCircle(int x, int y, int radius, int r, int g, int b, int a);
	void DrawRectRainbow(int x, int y, int width, int height, float flSpeed, float &flRainbow);
	void DrawTextureByteArray(Vector2D p0, Vector2D p1, Vector2D p2, Vector2D p3, const unsigned char* pRawRGBAData);
	unsigned int m_Font, font_esp, font_menu, font_build, font_GUI2, font_GUI, font_weapon;
	
};
extern HINSTANCE hAppInstance;
extern CDraw g_Draw;