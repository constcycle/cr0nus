#ifndef _HSKINS_H_
#define _HSKINS_H_

#pragma once

#include "HValve.h"

#pragma warning(disable : 4227)

namespace Skins
{
	struct SkinInfo_t
	{
		unsigned int    _id;        // index u know for what

		std::string     _shortname; // shortname
		std::string     _name;      // full skin name
	};

	typedef std::vector<SkinInfo_t> vecSkinInfo;
	typedef std::unordered_map<std::string, vecSkinInfo> mapSkinInfo;

	
	

		class CSkins
		{
		public:
			CSkins(void);
			~CSkins(void);
			bool								DumpTable(File::Valve::mapLevel tab, File::Valve::mapVariable vt, File::Valve::mapLevel ai_wi);
			bool                                Load(const std::string& gamePath, const std::string& gameShortName);
			void                                Release(void);
			void                                Dump(std::ofstream& dump);
			mapSkinInfo&						GetData();
			inline const mapSkinInfo& const GetSkinInfo(void) { return _skininfo; }
			inline const vecSkinInfo& const GetSkinInfoByWeapon(const std::string& weapon)
			{
				auto& ret = _skininfo.find(weapon);
				if (ret != _skininfo.end())
					return ret->second;
				return vecSkinInfo();
			}


		protected:
			File::Valve::CConfig*               _items_game = nullptr; // config file by valve
			File::Valve::CConfig*               _csgo_english = nullptr; // config file by valve
			mapSkinInfo                         _skininfo; // Holds Skinsinformation
		};
}

#pragma warning(default : 4227)

#endif /* _HSKINS_H_ */