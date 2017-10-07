#pragma once
class MTextbox : public MBaseElement
{
public:
	MTextbox() {}
	Vector pos;
	MBaseElement* parent;
	bool IsActive;
	std::string command;
	void HandleKeys(int key);
	MTextbox::MTextbox(MBaseElement* parent, std::string name, std::string caption, MCategory* SaveCategory, std::string savename);
	virtual void Draw() override;
};