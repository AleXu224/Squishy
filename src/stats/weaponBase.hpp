#pragma once

#include "curves.hpp"
#include "misc/weaponType.hpp"
#include "stats/stat.hpp"


namespace Stats {
	struct WeaponBase {
		const Misc::WeaponType type;
		const float baseAtk;
		const StatValue substat;
		const Curves::WeaponGrow atkCurve;
		const Curves::WeaponGrow substatCurve;
		const std::array<float, 7> ascensionUpgrade;

		[[nodiscard]] float getAtkAt(unsigned short level, unsigned short ascension) const {
			return (baseAtk * Curves::Weapon(atkCurve).at(level - 1)) + ascensionUpgrade.at(ascension);
		}

		[[nodiscard]] float getSubstatAt(unsigned short level) const {
			return substat.value * Curves::Weapon(substatCurve).at(level - 1);
		}
	};
}// namespace Stats
