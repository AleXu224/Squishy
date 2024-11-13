#pragma once

#include "Talents.hpp"
#include "array"
#include "curves.hpp"
#include "misc/element.hpp"
#include "misc/weaponType.hpp"
#include "stat.hpp"


namespace Stats {
	struct CharacterBase {
		const float baseHp;
		const float baseAtk;
		const float baseDef;
		const Stat ascensionStat;
		const uint8_t rarity;
		const ConstellationTalent c3Talent;
		const ConstellationTalent c5Talent;
		const Misc::Element element;
		const Misc::WeaponType weaponType;
		const Curves::CharacterGrow hpCurve;
		const Curves::CharacterGrow atkCurve;
		const Curves::CharacterGrow defCurve;
		const std::array<float, 7> hpUpgrade;
		const std::array<float, 7> atkUpgrade;
		const std::array<float, 7> defUpgrade;
		const std::array<float, 7> ascensionStatUpgrade;

		[[nodiscard]] constexpr float getHpAt(unsigned short level, unsigned short ascension) const {
			return (baseHp * Curves::Character(hpCurve).at(level - 1)) + hpUpgrade.at(ascension);
		}

		[[nodiscard]] constexpr float getAtkAt(unsigned short level, unsigned short ascension) const {
			return (baseAtk * Curves::Character(atkCurve).at(level - 1)) + atkUpgrade.at(ascension);
		}

		[[nodiscard]] constexpr float getDefAt(unsigned short level, unsigned short ascension) const {
			return (baseDef * Curves::Character(defCurve).at(level - 1)) + defUpgrade.at(ascension);
		}

		[[nodiscard]] constexpr float getAscensionStatAt(unsigned short ascension) const {
			return ascensionStatUpgrade.at(ascension);
		}
	};

}// namespace Stats
