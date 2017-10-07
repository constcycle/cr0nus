#pragma once
class MColorPicker : public MBaseElement
{
public:
	MColorPicker() {}
	bool is_open = false;
	MBaseElement* parent;
	DWORD color = D3DCOLOR_ARGB(255, 255, 255, 255);
	MColorPicker(MBaseElement* parent, std::string name, std::string caption,  MCategory* SaveCategory, std::string savename);
	virtual void Draw() override;
};