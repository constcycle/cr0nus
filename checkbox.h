class MCheckbox : public MBaseElement
{
public:
	MCheckbox() {}
	MBaseElement* parent;
	bool Checked;
	MCheckbox::MCheckbox(MBaseElement* parent, std::string name, std::string caption, MCategory* SaveCategory, string savename);
	virtual void Draw() override;
};