#pragma once

#include "Talents.hpp"
#include "formula/base.hpp"
#include "formula/index.hpp"
#include "modifiers/total/total.hpp"
#include "reaction/levelMultiplier.hpp"
#include "stats/loadout.hpp"
#include "utils/entryType.hpp"


namespace Formula {
	using namespace Formula::Operators;
	[[nodiscard]] inline const auto &_getMultiplier(LevelableTalent talent, const std::array<float, 15> &values, const Formula::Context &context) {
		switch (talent) {
			case LevelableTalent::normal:
				return values.at(Modifiers::totalTalents().normal.eval(context));
			case LevelableTalent::skill:
				return values.at(Modifiers::totalTalents().skill.eval(context));
			case LevelableTalent::burst:
				return values.at(Modifiers::totalTalents().burst.eval(context));
		}
		std::unreachable();
	}

	struct MultiplierValue : FormulaBase<float> {
		LevelableTalent talent;
		std::array<float, 15> values;
		Utils::EntryType entryType = Utils::EntryType::multiplier;

		[[nodiscard]] FloatNode fold(const Context &context, const FoldArgs &args) const {
			switch (talent) {
				case LevelableTalent::normal:
					return Index{.index = Modifiers::totalTalents().normal, .indexable = values}.fold(context, args);
				case LevelableTalent::skill:
					return Index{.index = Modifiers::totalTalents().skill, .indexable = values}.fold(context, args);
				case LevelableTalent::burst:
					return Index{.index = Modifiers::totalTalents().burst, .indexable = values}.fold(context, args);
			}
			std::unreachable();
		}

		[[nodiscard]] std::string print(const Context &context, Step) const {
			const auto &multiplier = _getMultiplier(talent, values, context);
			return Utils::printEntryType(multiplier, entryType);
		}

		[[nodiscard]] float eval(const Context &context) const {
			return _getMultiplier(talent, values, context);
		}
	};

	[[nodiscard]] inline auto Multiplier(auto stat, LevelableTalent talent, const std::array<float, 15> &values) {
		return stat * MultiplierValue({}, talent, values);
	}

	struct LevelMultiplier : FormulaBase<float, Type::constant> {
		[[nodiscard]] static std::string print(const Context &context, Step) {
			return Formula::Percentage("Level Multiplier", eval(context), true);
		}

		[[nodiscard]] static float eval(const Context &context) {
			return Reaction::CharacterLevelMultiplier.at(context.source.stats.sheet.level);
		}
	};

	[[nodiscard]] inline auto Multiplier(LevelableTalent talent, const std::array<float, 15> &values) {
		return MultiplierValue({}, talent, values, Utils::EntryType::points);
	}

	[[nodiscard]] inline auto Multiplier(Utils::EntryType type, LevelableTalent talent, const std::array<float, 15> &values) {
		return MultiplierValue({}, talent, values, type);
	}
}// namespace Formula