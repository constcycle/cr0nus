#pragma once
class MTab : public MBaseElement
{
public:
	MTab() {}
	MBaseElement* parent;
	std::vector<MBaseElement*> Children2;
	std::vector<std::string> Tabs;
	int number_tabs;
	int current_tab;
	MTab::MTab(MBaseElement* parent, int number_tabs, std::vector<std::string> TabNames);
	virtual void Draw() override;
};