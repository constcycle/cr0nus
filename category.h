#pragma once
class MCategory : public MBaseElement
{
public:
	MCategory() {}
	LPDIRECT3DTEXTURE9 texture;
	byte* tex_array;
	UINT texture_size;
	float res_x, res_y;
	MBaseElement* parent;
	static MCategory* CurrentCategory;
	std::vector<MBaseElement*> SaveChildren;
	bool active = false;
	MCategory(std::string name, MWindow * parent, byte* texture, UINT size, int res_x, int res_y);
	virtual void Draw() override;
};