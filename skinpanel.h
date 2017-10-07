#pragma once
#include "skinsutil.h"
class MSkinchanger : public MBaseElement
{
protected:
	Skins::vecSkinInfo currentWep;
	std::string currentWepName;
public:
	MSlider* Skinchanger_Wear;
	MSlider* Skinchanger_Seed;
	MCheckbox* Skinchanger_Stattrack;
	MSlider* Skinchanger_Stattrack_Kills;
	MTextbox* Skinchanger_Name;
	MButton* Skinchanger_Apply;
	MDropdown* Skinchanger_Knife;
	MDropdown* Skinchanger_Glove;
	MBaseElement* parent;
	MSkinchanger::MSkinchanger(MBaseElement* parent);
	virtual void Draw() override;
	void DrawList1();
	void DrawList2();
	bool Reload = false;
};
extern MSkinchanger* GlobalSkinchanger;
extern void ForceUpdate();