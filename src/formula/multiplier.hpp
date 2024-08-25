#pragma once

#include "Talents.hpp"
#include "character/data.hpp"
#include "formula/stat.hpp"
#include "intermediary.hpp"
#include "operators.hpp"
#include "stats/loadout.hpp"
#include "stats/stat.hpp"


namespace Formula {
	[[nodiscard]] inline const auto &_getMultiplier(LevelableTalent talent, const std::array<float, 15> &values, const Stats::Loadout &source) {
		switch (talent) {
			case LevelableTalent::normal:
				return values.at(source.character.sheet.talents.normal);
			case LevelableTalent::skill:
				return values.at(source.character.sheet.talents.skill);
			case LevelableTalent::burst:
				return values.at(source.character.sheet.talents.burst);
		}
		std::unreachable();
	}

	struct MultiplierValue {
		LevelableTalent talent;
		std::array<float, 15> values;

		[[nodiscard]] inline std::string print(const Stats::Loadout &source, const Stats::Loadout &, const Stats::Team &, Step) const {
			const auto &multiplier = _getMultiplier(talent, values, source);
			return fmt::format("{:.2f}%", multiplier * 100.f);
		}

		[[nodiscard]] inline float eval(const Stats::Loadout &source, const Stats::Loadout &, const Stats::Team &) const {
			return _getMultiplier(talent, values, source);
		}
	};

	[[nodiscard]] consteval auto Multiplier(::Stat stat, LevelableTalent talent, const std::array<float, 15> &values) {
		return Formula::Stat(stat) * MultiplierValue(talent, values);
	}
}// namespace Formula