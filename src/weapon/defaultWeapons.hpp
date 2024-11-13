#pragma once

#include "key.hpp"
#include "misc/weaponType.hpp"
#include "unordered_map"


namespace Weapon {
	const inline auto defaultWeapons = std::unordered_map<Misc::WeaponType, Weapon::DataKey>{
		{Misc::WeaponType::bow, {15101}},
		{Misc::WeaponType::catalyst, {14101}},
		{Misc::WeaponType::polearm, {13101}},
		{Misc::WeaponType::claymore, {12101}},
		{Misc::WeaponType::sword, {11101}},
	};
}