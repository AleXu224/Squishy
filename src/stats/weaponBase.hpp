#pragma once

#include "curves.hpp"
#include "misc/weaponType.hpp"
#include "stats/stat.hpp"
#include <optional>


namespace Stats {
	struct WeaponBase {
		struct SubStat {
			const StatValue stat;
			const Curves::WeaponGrow curve;
		};
		const Misc::WeaponType type;
		const uint8_t rarity;
		const float baseAtk;
		const Curves::WeaponGrow atkCurve;
		std::optional<SubStat> subStat;
		const std::array<float, 7> ascensionUpgrade;

		[[nodiscard]] float getAtkAt(unsigned short level, unsigned short ascension) const {
			return (baseAtk * Curves::Weapon(atkCurve).at(level - 1)) + ascensionUpgrade.at(ascension);
		}

		[[nodiscard]] float getSubstatAt(unsigned short level) const {
			if (!subStat.has_value()) return 0.f;
			const auto &substat = subStat.value();
			return substat.stat.value * Curves::Weapon(substat.curve).at(level - 1);
		}
	};
}// namespace Stats
