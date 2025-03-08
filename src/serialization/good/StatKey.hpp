#pragma once

#include "stats/stat.hpp"
#include "string"
#include "unordered_map"


namespace Serialization::Good {
	static inline std::unordered_map<std::string, Stat> statKey{
		{"hp", Stat::hp},
		{"hp_", Stat::hp_},
		{"atk", Stat::atk},
		{"atk_", Stat::atk_},
		{"def", Stat::def},
		{"def_", Stat::def_},
		{"eleMas", Stat::em},
		{"enerRech_", Stat::er},
		{"heal_", Stat::hb},
		{"critRate_", Stat::cr},
		{"critDMG_", Stat::cd},
		{"physical_dmg_", Stat::physicalDmg},
		{"anemo_dmg_", Stat::anemoDmg},
		{"geo_dmg_", Stat::geoDmg},
		{"electro_dmg_", Stat::electroDmg},
		{"hydro_dmg_", Stat::hydroDmg},
		{"pyro_dmg_", Stat::pyroDmg},
		{"cryo_dmg_", Stat::cryoDmg},
		{"dendro_dmg_", Stat::dendroDmg},
	};
	static inline std::unordered_map<Stat, std::string> keyStat{
		{Stat::hp, "hp"},
		{Stat::hp_, "hp_"},
		{Stat::atk, "atk"},
		{Stat::atk_, "atk_"},
		{Stat::def, "def"},
		{Stat::def_, "def_"},
		{Stat::em, "eleMas"},
		{Stat::er, "enerRech_"},
		{Stat::hb, "heal_"},
		{Stat::cr, "critRate_"},
		{Stat::cd, "critDMG_"},
		{Stat::physicalDmg, "physical_dmg_"},
		{Stat::anemoDmg, "anemo_dmg_"},
		{Stat::geoDmg, "geo_dmg_"},
		{Stat::electroDmg, "electro_dmg_"},
		{Stat::hydroDmg, "hydro_dmg_"},
		{Stat::pyroDmg, "pyro_dmg_"},
		{Stat::cryoDmg, "cryo_dmg_"},
		{Stat::dendroDmg, "dendro_dmg_"},
	};
}// namespace Serialization::Good