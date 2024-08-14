#pragma once

#include "Talents.hpp"
#include "character/data.hpp"
#include "intermediary.hpp"
#include "stats/sheet.hpp"
#include "stats/stat.hpp"
#include "formula/stat.hpp"
#include "operators.hpp"

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
		std::unreachable();
	}

	struct MultiplierValue {
		LevelableTalent talent;
		size_t index;

		[[nodiscard]] inline std::string print(const Stats::Sheet &stats, Step) const {
			auto &multiplier = _getMultiplier(talent, index, stats);
			return fmt::format("{:.2f}%", multiplier * 100.f);
		}

		[[nodiscard]] inline float eval(const Stats::Sheet &stats) const {
			return _getMultiplier(talent, index, stats);
		}
	};

	[[nodiscard]] consteval auto Multiplier(::Stat stat, LevelableTalent talent, size_t index) {
		return Formula::Stat(stat) * MultiplierValue(talent, index);
	}
}// namespace Formula