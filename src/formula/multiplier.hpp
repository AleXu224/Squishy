#pragma once

#include "Talents.hpp"
#include "formula/formulaContext.hpp"
#include "modifiers/total/total.hpp"
#include "reaction/levelMultiplier.hpp"
#include "stats/loadout.hpp"
#include "utils/entryType.hpp"

namespace Formula {
	using namespace Formula::Operators;
	[[nodiscard]] inline const auto &_getMultiplier(LevelableTalent talent, const std::array<float, 15> &values, const Formula::Context &context) {
		switch (talent) {
			case LevelableTalent::normal:
				return values.at(Modifiers::totalTalents.normal.eval(context));
			case LevelableTalent::skill:
				return values.at(Modifiers::totalTalents.skill.eval(context));
			case LevelableTalent::burst:
				return values.at(Modifiers::totalTalents.burst.eval(context));
		}
		std::unreachable();
	}

	struct MultiplierValue {
		LevelableTalent talent;
		std::array<float, 15> values;
		Utils::EntryType type = Utils::EntryType::multiplier;

		[[nodiscard]] std::string print(const Context &context, Step) const {
			const auto &multiplier = _getMultiplier(talent, values, context);
			return Utils::printEntryType(multiplier, type);
			std::unreachable();
		}

		[[nodiscard]] float eval(const Context &context) const {
			return _getMultiplier(talent, values, context);
		}
	};

	[[nodiscard]] consteval auto Multiplier(auto stat, LevelableTalent talent, const std::array<float, 15> &values) {
		return stat * MultiplierValue(talent, values);
	}

	struct LevelMultiplier {
		[[nodiscard]] static std::string print(const Context &context, Step) {
			return fmt::format("Level Multiplier {:.1f}", eval(context));
		}

		[[nodiscard]] static float eval(const Context &context) {
			return Reaction::CharacterLevelMultiplier.at(context.source.character.sheet.level);
		}
	};

	[[nodiscard]] consteval auto Multiplier(LevelableTalent talent, const std::array<float, 15> &values) {
		return MultiplierValue(talent, values, Utils::EntryType::points);
	}

	[[nodiscard]] consteval auto Multiplier(Utils::EntryType type, LevelableTalent talent, const std::array<float, 15> &values) {
		return MultiplierValue(talent, values, type);
	}
}// namespace Formula