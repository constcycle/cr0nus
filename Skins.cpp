#include "DynSkin\HSkins.h"

namespace Skins
{
	CSkins::CSkins(void)
	{
	}

	CSkins::~CSkins(void)
	{
		delete _items_game;
		delete _csgo_english;
	}

	bool CSkins::DumpTable(File::Valve::mapLevel tab, File::Valve::mapVariable vt, File::Valve::mapLevel ai_wi) {
		for (auto& pk : tab) {
			auto& pkid = pk.first;

			if (pkid == "9001")
				continue;

			auto& pkname = pk.second->GetVariables().at("name");
			auto& pkdesctag = std::string(pk.second->GetVariables().at("description_tag"));

			auto& res = vt.find(pkdesctag.substr(1, pkdesctag.size() + 1));
			if (res == vt.end()) {
				pkdesctag[6] = 'k';//FUCK IT DAMN VALVE L2SPELL FFS
				res = vt.find(pkdesctag.substr(1, pkdesctag.size() + 1));
				if (res == vt.end())
					continue;
			}

			auto& skname = res->second;
			if (skname == "-")
				continue;

			for (auto& wi : ai_wi) {
				auto& vip = wi.second->GetVariables().at("icon_path");
				if (vip[vip.size() - 7] != '_')
					continue;

				auto res = vip.find(pkname);
				if (res == std::string::npos)
					continue;

				_skininfo[vip.substr(23, res - 24)].push_back({ (unsigned int)atoi(pkid.c_str()), pkname, skname });
			}
		}

		return true;
	}

	bool CSkins::Load(const std::string& gamePath, const std::string& gameShortName)
	{
		Release();

		_items_game = new File::Valve::CConfig();
		if (!_items_game->Load(gamePath + "/scripts/items/items_game.txt", false))
			return false;

		_csgo_english = new File::Valve::CConfig();
		if (!_csgo_english->Load(gamePath + "/resource/" + gameShortName + "_english.txt", true))
			return false;

		// honestly this entire fix is so fucking ghetto kill me please
		auto fnFind = ([](std::multimap<std::string, File::Valve::CLevel*> map, std::string test) -> File::Valve::CLevel* {
			auto& lol = map.equal_range(test);
			for (auto it = lol.first; it != lol.second; ++it) {
#if defined (DEBUG)
				std::cout << "XD LOLOL found key " << it->first.c_str() << std::endl;
#endif
				return it->second;
			}
		});

		// ughgughu
		auto& vt = fnFind(_csgo_english->GetLevel()->GetSubLevels(), "Tokens")->GetVariables();
		auto mhhh = fnFind(_items_game->GetLevel()->GetSubLevels(), "alternate_icons2");
		auto mhh = fnFind(mhhh->GetSubLevels(), "weapon_icons");
		auto& ai_wi = mhh->GetSubLevels();

		for (auto& wat : _items_game->GetLevel()->GetSubLevels()) {
#if defined (DEBUG)
			std::cout << wat.first.c_str() << std::endl;
#endif
			if (wat.first.find("paint_kits") != std::string::npos) {
				// meh
				DumpTable(wat.second->GetSubLevels(), vt, ai_wi);
			}
		}

		return true;
	}

	void CSkins::Release(void)
	{
		if (_items_game)
			_items_game->Release();
		if (_csgo_english)
			_csgo_english->Release();
	}

	void CSkins::Dump(std::ofstream& dump)
	{
		for (auto& w : _skininfo) {
			dump << w.first.c_str() << std::endl;

			for (auto& s : w.second) {
				dump << "	" << s._name.c_str() << " -> " << s._id << "" << std::endl;
			}
		}
	}
	mapSkinInfo& CSkins::GetData()
	{
		return _skininfo;
	}
}
