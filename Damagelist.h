#pragma once
class MDamagelist : public MBaseElement
{
public:

	//MDamagelist() {}
	MBaseElement* parent;
	MDamagelist(MBaseElement* parent);

	virtual void Draw() override;

	std::vector<std::string> damage_infos;

};
extern MDamagelist* global_damagelist;