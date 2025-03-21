#pragma once

#include "element.hpp"
#include "fmt/core.h"
#include "formulaContext.hpp"
#include "modifiers/total/total.hpp"
#include "stats/loadout.hpp"
#include "step.hpp"


namespace Formula {
	struct EnemyDef {
		[[nodiscard]] static std::string print(const Context &context, Step) {
			return fmt::format("Enemy DEF {:.1f}%", eval(context));
		}

		[[nodiscard]] static float eval(const Context &context) {
			return ((1.f - Modifiers::totalEnemy.DEFReduction.eval(context)) * 5.f * Modifiers::totalEnemy.level.eval(context)) + 500.f;
		}
	};

	struct EnemyDefMultiplier {
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