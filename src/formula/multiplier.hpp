#pragma once

#include "Talents.hpp"
#include "formula/formulaContext.hpp"
#include "formula/stat.hpp"
#include "operators.hpp"
#include "reaction/levelMultiplier.hpp"
#include "stats/loadout.hpp"
#include "stats/stat.hpp"


namespace Formula {
	[[nodiscard]] inline const auto &_getMultiplier(LevelableTalent talent, const std::array<float, 15> &values, const Formula::Context &context) {
		switch (talent) {
			case LevelableTalent::normal:
				return values.at(context.source.character.sheet.talents.normal.get(context));
			case LevelableTalent::skill:
				return values.at(context.source.character.sheet.talents.skill.get(context));
			case LevelableTalent::burst:
				return values.at(context.source.character.sheet.talents.burst.get(context));
		}
		std::unreachable();
	}

	struct MultiplierValue {
		LevelableTalent talent;
		std::array<float, 15> values;

		[[nodiscard]] inline std::string print(const Context &context, Step) const {
			const auto &multiplier = _getMultiplier(talent, values, context);
			return fmt::format("{:.1f}%", multiplier * 100.f);
		}

		[[nodiscard]] inline float eval(const Context &context) const {
			return _getMultiplier(talent, values, context);
		}
	};

	[[nodiscard]] consteval auto Multiplier(::Stat stat, LevelableTalent talent, const std::array<float, 15> &values) {
		return Formula::CharacterStat(stat) * MultiplierValue(talent, values);
	}

	struct LevelMultiplier {
		[[nodiscard]] static inline std::string print(const Context &context, Step) {
			return fmt::format("Level Multiplier {:.1f}", eval(context));
		}

		[[nodiscard]] static inline float eval(const Context &context) {
			return Reaction::CharacterLevelMultiplier.at(context.source.character.sheet.level);
		}
	};
}// namespace Formula