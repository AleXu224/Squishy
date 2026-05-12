#pragma once

#include "elemental.hpp"
#include "formula/base.hpp"
#include "formula/requires.hpp"
#include "modifiers/total/total.hpp"
#include "stats/loadout.hpp"


namespace Formula {
	struct EnemyDef : FormulaBase<float> {
		[[nodiscard]] FloatNode fold(const Formula::Context &context, const FoldArgs &args) const {
			using namespace Operators;
			auto ret = ((1.f - Modifiers::totalEnemy().DEFReduction) * 5.f * Modifiers::totalEnemy().level) + ConstantFlat{.value = 500.f};
			return ret.fold(context, args);
		}

		[[nodiscard]] static std::string print(const Context &context, Step) {
			return Formula::Percentage("Enemy DEF", eval(context), true);
		}

		[[nodiscard]] static float eval(const Context &context) {
			return ((1.f - Modifiers::totalEnemy().DEFReduction.eval(context)) * 5.f * Modifiers::totalEnemy().level.eval(context)) + 500.f;
		}
	};

	struct EnemyDefMultiplier : FormulaBase<float> {
		[[nodiscard]] FloatNode fold(const Formula::Context &context, const FoldArgs &args) const {
			using namespace Operators;
			const auto characterLevel = ConstantFlat{.value = static_cast<float>(context.source.stats.sheet.level)};
			const auto enemyLevel = Modifiers::totalEnemy().level;
			const auto k = (1.f - Modifiers::totalEnemy().DEFReduction) * (1.f - Modifiers::totalEnemy().DEFIgnored);

			auto ret = (characterLevel + ConstantFlat{.value = 100.f}) / (k * (enemyLevel + ConstantFlat{.value = 100.f}) + (characterLevel + ConstantFlat{.value = 100.f}));
			return ret.fold(context, args);
		}

		[[nodiscard]] static std::string print(const Context &context, Step) {
			return Formula::Percentage("Enemy DEF Multiplier", eval(context), true);
		}

		[[nodiscard]] static float eval(const Context &context) {
			const auto characterLevel = static_cast<float>(context.source.stats.sheet.level);
			const auto enemyLevel = Modifiers::totalEnemy().level.eval(context);
			const auto k = (1.f - Modifiers::totalEnemy().DEFReduction.eval(context)) * (1.f - Modifiers::totalEnemy().DEFIgnored.eval(context));

			return (characterLevel + 100.f) / (k * (enemyLevel + 100.f) + (characterLevel + 100.f));
		}
	};

	struct EnemyResMultiplier : FormulaBase<float> {
		Utils::JankyOptional<Misc::AttackSource> attackSource{};
		Utils::JankyOptional<Misc::Element> element;

		[[nodiscard]] FloatNode fold(const Context &context, const FoldArgs &args) const {
			using namespace Operators;
			// Note: as of version 5.5 this is guaranteed to be alright but in the future if there is any character that has either
			// an infusion or res shred that relies on artifact stats then this will break
			const auto attackElement = getElement(attackSource, element, context);
			auto RES = Stats::fromEnemyResElement(Modifiers::totalEnemy().resistance, attackElement);

			auto ret = IfElse{
				.requirement = RES < 0.f,
				.trueVal = 1.f - (RES / 2.f),
				.elseVal = IfElse{
					.requirement = RES < 0.75f,
					.trueVal = 1.f - RES,
					.elseVal = 1.f - (4.f * RES + 1.f),
				},
			};
			return ret.fold(context, args);
		}

		[[nodiscard]] std::string print(const Context &context, Step) const {
			return Formula::Percentage("Enemy RES Multiplier", eval(context), true);
		}

		[[nodiscard]] float eval(const Context &context) const {
			const auto attackElement = getElement(attackSource, element, context);
			auto RES = Stats::fromEnemyResElement(Modifiers::totalEnemy().resistance, attackElement).eval(context);

			if (RES < 0.f) return 1.f - (RES / 2.f);
			if (RES < 0.75f) return 1.f - RES;
			return 1.f - (4.f * RES + 1.f);
		}
	};
}// namespace Formula