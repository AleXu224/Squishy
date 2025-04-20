#pragma once

#include "compiled/requires.hpp"
#include "element.hpp"
#include "fmt/core.h"
#include "formulaContext.hpp"
#include "modifiers/total/total.hpp"
#include "stats/loadout.hpp"
#include "step.hpp"


namespace Formula {
	struct EnemyDef {
		[[nodiscard]] auto compile(const Formula::Context &context) const {
			using namespace Compiled::Operators;
			return ((Compiled::ConstantFloat(1.f) - Modifiers::totalEnemy.DEFReduction.compile(context)) * Compiled::ConstantFloat(5.f) * Modifiers::totalEnemy.level.compile(context)) + Compiled::ConstantFloat(500.f);
		}

		[[nodiscard]] static std::string print(const Context &context, Step) {
			return fmt::format("Enemy DEF {:.1f}%", eval(context));
		}

		[[nodiscard]] static float eval(const Context &context) {
			return ((1.f - Modifiers::totalEnemy.DEFReduction.eval(context)) * 5.f * Modifiers::totalEnemy.level.eval(context)) + 500.f;
		}
	};

	struct EnemyDefMultiplier {
		[[nodiscard]] auto compile(const Formula::Context &context) const {
			using namespace Compiled::Operators;
			const auto characterLevel = Compiled::ConstantFloat(context.source.character.sheet.level);
			const auto enemyLevel = Modifiers::totalEnemy.level.compile(context);
			const auto k = (Compiled::ConstantFloat(1.f) - Modifiers::totalEnemy.DEFReduction.compile(context)) * (Compiled::ConstantFloat(1.f) - Modifiers::totalEnemy.DEFIgnored.compile(context));

			return (characterLevel + Compiled::ConstantFloat(100.f)) / (k * (enemyLevel + Compiled::ConstantFloat(100.f)) + (characterLevel + Compiled::ConstantFloat(100.f)));
		}

		[[nodiscard]] static std::string print(const Context &context, Step) {
			return fmt::format("Enemy DEF Multiplier {:.1f}%", eval(context) * 100.f);
		}

		[[nodiscard]] static float eval(const Context &context) {
			const auto characterLevel = static_cast<float>(context.source.character.sheet.level);
			const auto enemyLevel = Modifiers::totalEnemy.level.eval(context);
			const auto k = (1.f - Modifiers::totalEnemy.DEFReduction.eval(context)) * (1.f - Modifiers::totalEnemy.DEFIgnored.eval(context));

			return (characterLevel + 100.f) / (k * (enemyLevel + 100.f) + (characterLevel + 100.f));
		}
	};

	struct EnemyResMultiplier {
		Misc::AttackSource attackSource{};
		Utils::JankyOptional<Misc::Element> element;

		[[nodiscard]] auto compile(const Context &context) const {
			using namespace Compiled::Operators;
			// Note: as of version 5.5 this is guaranteed to be alright but in the future if there is any character that has either
			// an infusion or res shred that relies on artifact stats then this will break
			const auto attackElement = getElement(attackSource, element, context);
			auto RES = Stats::evalEnemyResElement<Modifiers::totalEnemy.resistance>(attackElement, context);

			return Compiled::IfElseMaker(
				Compiled::ConstantFloat(RES) < Compiled::ConstantFloat(0.f),
				Compiled::ConstantFloat(1.f) - (Compiled::ConstantFloat(RES) / Compiled::ConstantFloat(2.f)),
				Compiled::IfElseMaker(
					Compiled::ConstantFloat(RES) < Compiled::ConstantFloat(0.75f),
					Compiled::ConstantFloat(1.f) - Compiled::ConstantFloat(RES),
					Compiled::ConstantFloat(1.f) - (Compiled::ConstantFloat(4.f) * Compiled::ConstantFloat(RES) + Compiled::ConstantFloat(1.f))
				)
			);
		}

		[[nodiscard]] std::string print(const Context &context, Step) const {
			return fmt::format("Enemy RES Multiplier {:.1f}%", eval(context) * 100.f);
		}

		[[nodiscard]] float eval(const Context &context) const {
			const auto attackElement = getElement(attackSource, element, context);
			auto RES = Stats::evalEnemyResElement<Modifiers::totalEnemy.resistance>(attackElement, context);

			if (RES < 0.f) return 1.f - (RES / 2.f);
			if (RES < 0.75f) return 1.f - RES;
			return 1.f - (4.f * RES + 1.f);
		}
	};
}// namespace Formula