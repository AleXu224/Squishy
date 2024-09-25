#pragma once

#include "cstdint"
#include "curves.hpp"
#include "stats/stat.hpp"


enum class WeaponType : uint8_t {
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
