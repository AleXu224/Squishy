#pragma once

#include "Curves.hpp"
#include "Element.hpp"
#include "array"
#include "stat.hpp"


enum class ConstellationTalent {
	normal,
	skill,
	burst,
};

namespace Stats {
	struct CharacterBase {
		const float baseHp;
		const float baseAtk;
		const float baseDef;
		const Stat ascensionStat;
		const ConstellationTalent c3Talent;
		const ConstellationTalent c5Talent;
		const Element element;
		const CharacterCurveType hpCurve;
		const CharacterCurveType atkCurve;
		const CharacterCurveType defCurve;
		const std::array<float, 7> hpUpgrade;
		const std::array<float, 7> atkUpgrade;
		const std::array<float, 7> defUpgrade;
		const std::array<float, 7> ascensionStatUpgrade;

		[[nodiscard]] float getHpAt(unsigned short level, unsigned short ascension) const {
			return baseHp * CharacterCurves.at(hpCurve).at(level - 1) + hpUpgrade.at(ascension);
		}

		[[nodiscard]] float getAtkAt(unsigned short level, unsigned short ascension) const {
			return baseAtk * CharacterCurves.at(atkCurve).at(level - 1) + atkUpgrade.at(ascension);
		}

		[[nodiscard]] float getDefAt(unsigned short level, unsigned short ascension) const {
			return baseDef * CharacterCurves.at(defCurve).at(level - 1) + defUpgrade.at(ascension);
		}

		[[nodiscard]] float getAscensionStatAt(unsigned short ascension) const {
			return ascensionStatUpgrade.at(ascension);
		}
	};

}// namespace Stats
