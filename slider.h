#pragma once
class MSlider : public MBaseElement
{
public:
	MSlider() {}
	
	bool bActive;
	MBaseElement* parent;
	float min, max, value;
	bool round;
	int width;
	MSlider(MBaseElement* parent, std::string name, std::string caption, float min, float max, bool round, MCategory* SaveCategory, std::string savename);
	virtual void Draw() override;
};