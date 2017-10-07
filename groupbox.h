#pragma once
class MGroupbox : public MBaseElement
{
public:
	MBaseElement* parent;
	bool inside;
	int x_offset,  y_offset, y_blocks;
	float x_blocks;
	std::string caption;
	MGroupbox::MGroupbox(MBaseElement* parent, std::string caption, int x_offset, int y_offset, float x_blocks, int y_blocks, bool inside);
	virtual void Draw() override;
};