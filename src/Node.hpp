#pragma once

#include "Element.hpp"
#include "algorithm"
#include "functional"
#include "stats/character.hpp"
#include "stats/characterSheet.hpp"
#include "stats/stat.hpp"
#include <format>


enum class AttackType {
	normal,
	charged,
	plunge,
	skill,
	burst,
};

enum class NodeType {
	attack,
	// heal,
	// shield,
};

namespace Character {
	struct Data;
}

struct Node {
	std::string_view name = "## NODE_NAME ##";
	NodeType nodeType = NodeType::attack;
	AttackType attackType;
	Element attackElement;
	// bool isCompletelyCustom;
	bool isPercentage = false;
	std::function<void(const Character::Data &characterData, Stats::Character &stats)> modifier{};
	std::function<float(const Character::Data &characterData, Stats::Character &stats)> formula;

	std::string getFormattedValue(const Character::Data &characterData, Stats::Character &stats) const {
		if (isPercentage)
			return std::format("{:.2f}%", 100.0f * getValue(characterData, stats));

		return std::format("{:.2f}", getValue(characterData, stats));
	}

	float getValue(const Character::Data &characterData, Stats::Character &stats) const {
		switch (nodeType) {
			case NodeType::attack: {
				Stats::CharacterSheet &s = stats.sheet;
				//				#ifndef NDEBUG
				//				printf("(%f + %f Additional dmg) *\n", formula(characterData, stats), s.getStat(Stat::additionalAtk));
				//				printf("(1.0 + %f Elem Dmg bonus + %f All dmg bonus) *\n", s.getStatFromElement(attackElement), s.getStat(Stat::allDmg));
				//				printf("(1.0 + %f Crit Rate * %f Crit Dmg) *\n",  std::clamp(s.getStat(Stat::cr), 0.0f, 1.0f), s.getStat(Stat::cd));
				//				printf("0.5 * (1.0 - 0.1)\n");
				//				#endif
				return (
					(formula(characterData, stats) + s.getStat(Stat::additionalAtk)) *
					(1.0f + s.getStatFromElement(attackElement) + s.getStat(Stat::allDmg)) *
					(1.0f + std::clamp(s.getStat(Stat::cr), 0.0f, 1.0f) * s.getStat(Stat::cd)) *
					0.5f *       // Enemy def
					(1.0f - 0.1f)// Enemy dmg res
				);
			}
		}
	}
};
