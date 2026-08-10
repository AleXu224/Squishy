#pragma once

#include "attribute.hpp"
#include "formula/attackModifier.hpp"
#include "formula/base.hpp"
#include "formula/cast.hpp"
#include "formula/clamp.hpp"
#include "formula/index.hpp"
#include "modifiers/total/total.hpp"
#include "stats/loadout.hpp"


namespace Formula {
	struct EnemyDefMultiplier : FormulaBase<float> {
		EnemyModifier modifiers{};

		auto getFormula(const Formula::Context &context) const {
			using namespace Operators;
			const auto &enemy = Modifiers::enemy();
			auto baseDef = Index{
							   .index = Cast<int32_t, FloatNode>{.formula = Clamp{.val1 = enemy.level + modifiers.level, .min = 1.f, .max = 60.f}}
									  - ConstantInt{.value = 1},
							   .indexable = Curves::levelMultiplier,
						   }
						 * (enemy.baseDef + modifiers.baseDef);
			const auto def = baseDef * (1.f - (enemy.DEFReduction + modifiers.DEFReduction) - (enemy.DEFIgnored + modifiers.DEFIgnored));
			const auto effectiveDef = def * (1.f - Modifiers::combat().penRatio) - Modifiers::combat().pen;
			auto levelCoeff = Curves::levelMultiplier.at(context.source.stats.sheet.level - 1) * 50.f;
			auto defMod = levelCoeff / (levelCoeff + effectiveDef);

			return defMod;
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
			const auto &enemy = Modifiers::enemy();
			auto baseDef = Curves::levelMultiplier.at(std::clamp(static_cast<int32_t>(enemy.level.eval(context) + modifiers.level.eval(context)), 0, 60) - 1) * (enemy.baseDef.eval(context) + modifiers.baseDef.eval(context));
			const auto def = baseDef * (1.f - (enemy.DEFReduction.eval(context) + modifiers.DEFReduction.eval(context)) - (enemy.DEFIgnored.eval(context) + modifiers.DEFIgnored.eval(context)));
			const auto effectiveDef = def * (1.f - Modifiers::combat().penRatio.eval(context)) - Modifiers::combat().pen.eval(context);
			auto levelCoeff = Curves::levelMultiplier.at(context.source.stats.sheet.level - 1) * 50.f;
			auto defMod = levelCoeff / (levelCoeff + effectiveDef);

			return defMod;
		}
	};

	struct EnemyResMultiplier : FormulaBase<float> {
		Utils::JankyOptional<Misc::Attribute> element{};
		EnemyModifierResistance modifiers{};

		auto getFormula(const Formula::Context &context) const {
			using namespace Operators;
			const auto attackElement = getAttribute(element, context);
			auto RES = Stats::fromEnemyResAttribute(Modifiers::enemy().resistance, attackElement);
			auto RESModifier = Stats::fromEnemyResAttribute(modifiers, attackElement);

			auto ret = Min{
				.val1 = Constant{.value = 2.f},
				.val2 = 1.f - (RES + RESModifier),
			};
			return ret;
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
			const auto attackElement = getAttribute(element, context);
			auto RES = Stats::fromEnemyResAttribute(Modifiers::enemy().resistance, attackElement).eval(context);
			auto RESModifier = Stats::fromEnemyResAttribute(modifiers, attackElement).eval(context);

			auto ret = std::min(2.f, 1.f - (RES + RESModifier));
			return ret;
		}
	};

	// Used when the element is not known at compile time
	struct EnemyResMultiplierDynamic : FormulaBase<float> {
		NodeType<Misc::Attribute> element{};
		EnemyModifierResistance modifiers{};

		[[nodiscard]] FloatNode fold(const Context &context, const FoldArgs &args) const {
			using namespace Operators;
			auto attackElement = element.fold(context, args);
			if (attackElement.getType() == Type::constant) {
				return EnemyResMultiplier{.element = attackElement.getConstantValue(), .modifiers = modifiers}.fold(context, args);
			}
			return *this;
		}

		void print(Descriptor &descriptor, const Context &context, Step) const {
			auto attackElement = element.eval(context);
			auto formula = EnemyResMultiplier{.element = attackElement, .modifiers = modifiers};
			formula.print(descriptor, context, Step::none);
		}

		[[nodiscard]] float eval(const Context &context) const {
			auto attackElement = element.eval(context);
			return EnemyResMultiplier{.element = attackElement, .modifiers = modifiers}.eval(context);
		}
	};
}// namespace Formula