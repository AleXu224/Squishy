#pragma once

#include "stats/stat.hpp"
#include "string"
#include "unordered_map"


namespace Serialization::Zod {
	static inline std::unordered_map<std::string, Stat> statKey{
		{"hp", Stat::hp},
		{"hp_", Stat::hp_},
		{"atk", Stat::atk},
		{"atk_", Stat::atk_},
		{"def", Stat::def},
		{"def_", Stat::def_},
		{"enerRegen_", Stat::er_},
		{"anomProf", Stat::ap},
		{"anomMas_", Stat::am_},
		{"crit_", Stat::cr},
		{"crit_dmg_", Stat::cd},
		{"pen", Stat::pen},
		{"pen_", Stat::penRatio},
		{"impact_", Stat::impact_},
		{"physical_dmg_", Stat::physicalDmg},
		{"fire_dmg_", Stat::fireDmg},
		{"ice_dmg_", Stat::iceDmg},
		{"electric_dmg_", Stat::electricDmg},
		{"ether_dmg_", Stat::etherDmg},
	};
	static inline std::unordered_map<Stat, std::string> keyStat{
		{Stat::hp, "hp"},
		{Stat::hp_, "hp_"},
		{Stat::atk, "atk"},
		{Stat::atk_, "atk_"},
		{Stat::def, "def"},
		{Stat::def_, "def_"},
		{Stat::er, "enerRegen_"},
		{Stat::ap, "anomProf"},
		{Stat::am_, "anomMas_"},
		{Stat::cr, "crit_"},
		{Stat::cd, "crit_dmg_"},
		{Stat::pen, "pen"},
		{Stat::penRatio, "pen_"},
		{Stat::impact_, "impact_"},
		{Stat::physicalDmg, "physical_dmg_"},
		{Stat::fireDmg, "fire_dmg_"},
		{Stat::iceDmg, "ice_dmg_"},
		{Stat::electricDmg, "electric_dmg_"},
		{Stat::etherDmg, "ether_dmg_"},
	};
}// namespace Serialization::Zod