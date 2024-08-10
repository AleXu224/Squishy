#pragma once

#include "Talents.hpp"
#include "character/data.hpp"
#include "intermediary.hpp"
#include "stats/sheet.hpp"


namespace Formula {
	[[nodiscard]] inline const auto &_getMultiplier(LevelableTalent talent, size_t index, const Stats::Sheet &stats) {
		switch (talent) {
			case LevelableTalent::normal:
				return stats.character.data.multipliers.normal.at(index).at(stats.character.sheet.talents.normal);
			case LevelableTalent::skill:
				return stats.character.data.multipliers.skill.at(index).at(stats.character.sheet.talents.skill);
			case LevelableTalent::burst:
				return stats.character.data.multipliers.burst.at(index).at(stats.character.sheet.talents.burst);
		}
	}

	[[nodiscard]] consteval auto SkillMultiplier(LevelableTalent talent, size_t index) {
		return Intermediary{
			.print = [talent, index](const Stats::Sheet &stats, Step) -> std::string {
				auto &multiplier = _getMultiplier(talent, index, stats);
				return fmt::format("{:.2f}%", multiplier * 100.f);
			},
			.eval = [talent, index](const Stats::Sheet &stats) -> float {
				return _getMultiplier(talent, index, stats);
			},
		};
	}
}// namespace Formula