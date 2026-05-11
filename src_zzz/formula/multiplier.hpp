#pragma once

#include "formula/base.hpp"
#include "formula/cast.hpp"
#include "formula/index.hpp"
#include "formula/operators.hpp"
#include "modifiers/total/total.hpp"
#include "stats/skill.hpp"
#include "utils/entryType.hpp"


namespace Formula {
	using namespace Formula::Operators;
	[[nodiscard]] inline float _getMultiplier(LevelableSkill skill, float base, float grow, const Formula::Context &context) {
		switch (skill) {
			case LevelableSkill::basic:
				return base + grow * Modifiers::skills().basic.eval(context);
			case LevelableSkill::dodge:
				return base + grow * Modifiers::skills().dodge.eval(context);
			case LevelableSkill::assist:
				return base + grow * Modifiers::skills().assist.eval(context);
			case LevelableSkill::special:
				return base + grow * Modifiers::skills().special.eval(context);
			case LevelableSkill::chain:
				return base + grow * Modifiers::skills().chain.eval(context);
			case LevelableSkill::core:
				return base + grow * Modifiers::skills().core.eval(context);
		}
		std::unreachable();
	}

	struct MultiplierValue : FormulaBase<float> {
		LevelableSkill skill;
		float base;
		float grow;
		Utils::EntryType entryType = Utils::EntryType::multiplier;

		[[nodiscard]] FloatNode fold(const Context &context, const FoldArgs &args) const {
			auto b = Constant{.value = base};
			auto g = Constant{.value = grow};
			auto ret = [&]() {
				switch (skill) {
					case LevelableSkill::basic:
						return b + g * Cast<float, IntNode>{.formula = Modifiers::skills().basic};
					case LevelableSkill::dodge:
						return b + g * Cast<float, IntNode>{.formula = Modifiers::skills().dodge};
					case LevelableSkill::assist:
						return b + g * Cast<float, IntNode>{.formula = Modifiers::skills().assist};
					case LevelableSkill::special:
						return b + g * Cast<float, IntNode>{.formula = Modifiers::skills().special};
					case LevelableSkill::chain:
						return b + g * Cast<float, IntNode>{.formula = Modifiers::skills().chain};
					case LevelableSkill::core:
						return b + g * Cast<float, IntNode>{.formula = Modifiers::skills().core};
				}
			}();
			return ret.fold(context, args);
		}

		[[nodiscard]] std::string print(const Context &context, Step) const {
			const auto &multiplier = _getMultiplier(skill, base, grow, context);
			return Utils::printEntryType(multiplier, entryType);
		}

		[[nodiscard]] float eval(const Context &context) const {
			return _getMultiplier(skill, base, grow, context);
		}
	};

	[[nodiscard]] inline auto Multiplier(auto stat, LevelableSkill skill, float base, float grow) {
		return stat * MultiplierValue({}, skill, base, grow);
	}

	[[nodiscard]] inline auto Multiplier(LevelableSkill skill, float base, float grow) {
		return MultiplierValue({}, skill, base, grow, Utils::EntryType::points);
	}

	[[nodiscard]] inline auto Multiplier(Utils::EntryType type, LevelableSkill skill, float base, float grow) {
		return MultiplierValue({}, skill, base, grow, type);
	}

	[[nodiscard]] static inline auto CoreMultiplier(bool isPercentage, const std::array<float, 7> &values) {
		return Formula::Index{
			.index = Modifiers::skills().core,
			.isPercentage = isPercentage,
			.indexable = values,
		};
	}
}// namespace Formula