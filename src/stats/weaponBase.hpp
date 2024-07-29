#pragma once

#include "Curves.hpp"
#include "stats/stat.hpp"


enum class WeaponType {
	sword,
	claymore,
	polearm,
	catalyst,
	bow,
};

namespace Stats {
	struct WeaponBase {
		const WeaponType type;
		const float baseAtk;
		const StatValue substat;
		const WeaponCurveType atkCurve;
		const WeaponCurveType substatCurve;
		const std::array<float, 7> ascensionUpgrade;

		[[nodiscard]] float getAtkAt(unsigned short level, unsigned short ascension) const {
			return baseAtk * WeaponCurves.at(atkCurve).at(level - 1) + ascensionUpgrade.at(ascension);
		}

		[[nodiscard]] float getSubstatAt(unsigned short level) const {
			return substat.value * WeaponCurves.at(substatCurve).at(level - 1);
		}
	};
}// namespace Stats
