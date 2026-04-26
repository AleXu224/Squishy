#pragma once

#include "attribute.hpp"
#include "formula/base.hpp"
#include "formula/cast.hpp"
#include "formula/clamp.hpp"
#include "formula/index.hpp"
#include "modifiers/total/total.hpp"
#include "stats/loadout.hpp"


namespace Formula {
	struct EnemyDefMultiplier : FormulaBase<float> {
		[[nodiscard]] FloatNode fold(const Formula::Context &context, const FoldArgs &args) const {
			using namespace Operators;
			const auto &enemy = Modifiers::enemy();
			auto baseDef = Index{
							   .index = Cast<int32_t, FloatNode>{.formula = Clamp{.val1 = enemy.level, .min = 1.f, .max = 60.f}}
									  - ConstantInt{.value = 1},
							   .indexable = Curves::levelMultiplier,
						   }
						 * enemy.baseDef;
			const auto def = baseDef * (1.f - enemy.DEFReduction - enemy.DEFIgnored);
			const auto effectiveDef = def * (1.f - Modifiers::combat().penRatio) - Modifiers::combat().pen;
			auto levelCoeff = Curves::levelMultiplier.at(context.active.stats.sheet.level - 1) * 50.f;
			auto defMod = levelCoeff / (levelCoeff + effectiveDef);

			return defMod.fold(context, args);
		}

		[[nodiscard]] static std::string print(const Context &context, Step) {
			return fmt::format("Enemy DEF Multiplier {:.1f}%", eval(context) * 100.f);
		}

		[[nodiscard]] static float eval(const Context &context) {
			const auto &enemy = Modifiers::enemy();
			auto baseDef = Curves::levelMultiplier.at(std::clamp(static_cast<int32_t>(enemy.level.eval(context)), 0, 60) - 1) * enemy.baseDef.eval(context);
			const auto def = baseDef * (1.f - enemy.DEFReduction.eval(context) - enemy.DEFIgnored.eval(context));
			const auto effectiveDef = def * (1.f - Modifiers::combat().penRatio.eval(context)) - Modifiers::combat().pen.eval(context);
			auto levelCoeff = Curves::levelMultiplier.at(context.active.stats.sheet.level - 1) * 50.f;
			auto defMod = levelCoeff / (levelCoeff + effectiveDef);

			return defMod;
		}
	};

	struct EnemyResMultiplier : FormulaBase<float> {
		Utils::JankyOptional<Misc::AttackSource> attackSource{};
		Utils::JankyOptional<Misc::Attribute> element;

		[[nodiscard]] FloatNode fold(const Context &context, const FoldArgs &args) const {
			using namespace Operators;
			// Note: as of version 5.5 this is guaranteed to be alright but in the future if there is any character that has either
			// an infusion or res shred that relies on artifact stats then this will break
			const auto attackElement = getAttribute(attackSource, element, context);
			auto RES = Stats::fromEnemyResAttribute(Modifiers::enemy().resistance, attackElement);

			auto ret = Min{
				.val1 = Constant{.value = 2.f},
				.val2 = 1.f - RES,
			};
			return ret.fold(context, args);
		}

		[[nodiscard]] std::string print(const Context &context, Step) const {
			return fmt::format("Enemy RES Multiplier {:.1f}%", eval(context) * 100.f);
		}

		[[nodiscard]] float eval(const Context &context) const {
			const auto attackElement = getAttribute(attackSource, element, context);
			auto RES = Stats::fromEnemyResAttribute(Modifiers::enemy().resistance, attackElement).eval(context);

			auto ret = std::min(2.f, 1.f - RES);
			return ret;
		}
	};
}// namespace Formula