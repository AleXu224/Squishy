#pragma once

#include "elemental.hpp"
#include "formula/attackModifier.hpp"
#include "formula/base.hpp"
#include "formula/requires.hpp"
#include "modifiers/total/total.hpp"
#include "stats/loadout.hpp"


namespace Formula {
	struct EnemyDef : FormulaBase<float> {
		static auto getFormula() {
			return ((1.f - Modifiers::totalEnemy().DEFReduction) * 5.f * Modifiers::totalEnemy().level) + ConstantFlat{.value = 500.f};
		}

		[[nodiscard]] FloatNode fold(const Formula::Context &context, const FoldArgs &args) const {
			using namespace Operators;
			return getFormula().fold(context, args);
		}

		static void print(Descriptor &descriptor, const Context &context, Step) {
			Descriptor formulaDescriptor;
			getFormula().print(formulaDescriptor, context, Step::none);
			descriptor.add("Enemy DEF", {eval(context), false}, formulaDescriptor);
		}

		[[nodiscard]] static float eval(const Context &context) {
			return ((1.f - Modifiers::totalEnemy().DEFReduction.eval(context)) * 5.f * Modifiers::totalEnemy().level.eval(context)) + 500.f;
		}
	};

	struct EnemyDefMultiplier : FormulaBase<float> {
		EnemyModifier modifiers{};

		auto getFormula(const Formula::Context &context) const {
			using namespace Operators;
			const auto characterLevel = ConstantFlat{.value = static_cast<float>(context.source.stats.sheet.level)};
			const auto enemyLevel = Modifiers::totalEnemy().level + modifiers.level;
			const auto k = (1.f - (Modifiers::totalEnemy().DEFReduction + modifiers.DEFReduction)) * (1.f - (Modifiers::totalEnemy().DEFIgnored + modifiers.DEFIgnored));

			return (characterLevel + ConstantFlat{.value = 100.f}) / (k * (enemyLevel + ConstantFlat{.value = 100.f}) + (characterLevel + ConstantFlat{.value = 100.f}));
		}

		[[nodiscard]] FloatNode fold(const Formula::Context &context, const FoldArgs &args) const {
			return getFormula(context).fold(context, args);
		}

		void print(Descriptor &descriptor, const Context &context, Step) const {
			Descriptor formulaDescriptor;
			getFormula(context).print(formulaDescriptor, context, Step::none);
			descriptor.add("Enemy DEF Multiplier", {eval(context), true}, formulaDescriptor);
		}

		[[nodiscard]] float eval(const Context &context) const {
			const auto characterLevel = static_cast<float>(context.source.stats.sheet.level);
			const auto enemyLevel = Modifiers::totalEnemy().level.eval(context) + modifiers.level.eval(context);
			const auto k = (1.f - (Modifiers::totalEnemy().DEFReduction.eval(context) + modifiers.DEFReduction.eval(context))) * (1.f - (Modifiers::totalEnemy().DEFIgnored.eval(context) + modifiers.DEFIgnored.eval(context)));

			return (characterLevel + 100.f) / (k * (enemyLevel + 100.f) + (characterLevel + 100.f));
		}
	};

	struct EnemyResMultiplier : FormulaBase<float> {
		Utils::JankyOptional<Misc::AttackSource> attackSource{};
		Utils::JankyOptional<Misc::Element> element;
		EnemyModifierResistance modifiers{};

		auto getFormula(const Formula::Context &context) const {
			using namespace Operators;
			// Note: as of version 5.5 this is guaranteed to be alright but in the future if there is any character that has either
			// an infusion or res shred that relies on artifact stats then this will break
			const auto attackElement = getElement(attackSource, element, context);
			auto RES = Stats::fromEnemyResElement(Modifiers::totalEnemy().resistance, attackElement);
			auto RESModifier = Stats::fromEnemyResElement(modifiers, attackElement);
			auto totalRES = RES + RESModifier;

			return IfElse{
				.requirement = totalRES < 0.f,
				.trueVal = 1.f - (totalRES / 2.f),
				.elseVal = IfElse{
					.requirement = totalRES < 0.75f,
					.trueVal = 1.f - totalRES,
					.elseVal = 1.f - (4.f * totalRES + 1.f),
				},
			};
		}

		[[nodiscard]] FloatNode fold(const Context &context, const FoldArgs &args) const {
			return getFormula(context).fold(context, args);
		}

		void print(Descriptor &descriptor, const Context &context, Step) const {
			Descriptor formulaDescriptor;
			getFormula(context).print(formulaDescriptor, context, Step::none);
			descriptor.add("Enemy RES Multiplier", {eval(context), true}, formulaDescriptor);
		}

		[[nodiscard]] float eval(const Context &context) const {
			const auto attackElement = getElement(attackSource, element, context);
			auto RES = Stats::fromEnemyResElement(Modifiers::totalEnemy().resistance, attackElement).eval(context);
			auto RESModifier = Stats::fromEnemyResElement(modifiers, attackElement).eval(context);
			auto totalRES = RES + RESModifier;

			if (totalRES < 0.f) return 1.f - (totalRES / 2.f);
			if (totalRES < 0.75f) return 1.f - totalRES;
			return 1.f - (4.f * totalRES + 1.f);
		}
	};
}// namespace Formula