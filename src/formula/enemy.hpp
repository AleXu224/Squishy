#pragma once

#include "fmt/core.h"
#include "formula/stat.hpp"
#include "formulaContext.hpp"
#include "stats/enemy.hpp"
#include "stats/loadout.hpp"
#include "step.hpp"


namespace Formula {
	struct EnemyDef {
		[[nodiscard]] static inline std::string print(const Context &context, Step) {
			return fmt::format("Enemy DEF {:.1f}%", eval(context));
		}

		[[nodiscard]] static inline float eval(const Context &context) {
			return (1.f - context.enemy.sheet.DEFReduction.get(context)) * 5.f * context.enemy.sheet.level.get(context) + 500.f;
		}
	};

	struct EnemyDefMultiplier {
		[[nodiscard]] static inline std::string print(const Context &context, Step) {
			return fmt::format("Enemy DEF Multiplier {:.1f}%", eval(context) * 100.f);
		}

		[[nodiscard]] static inline float eval(const Context &context) {
			const auto characterLevel = static_cast<float>(context.source.character.sheet.level);
			const auto enemyLevel = context.enemy.sheet.level.get(context);
			const auto k = (1.f - context.enemy.sheet.DEFReduction.get(context)) * (1.f - context.enemy.sheet.DEFReduction.get(context));

			return (characterLevel + 100.f) / (k * (enemyLevel + 100.f) + (characterLevel + 100.f));
		}
	};

	struct EnemyResMultiplier {
		Misc::AttackSource attackSource{};
		Utils::JankyOptional<Misc::Element> element;

		[[nodiscard]] inline std::string print(const Context &context, Step) const {
			return fmt::format("Enemy RES Multiplier {:.1f}%", eval(context) * 100.f);
		}

		[[nodiscard]] inline float eval(const Context &context) const {
			const auto attackElement = Formula::_getElement(attackSource, element, context);
			const auto &RES = context.enemy.sheet.resistance.fromElement(attackElement).get(context);

			if (RES < 0.f) return 1.f - (RES / 2.f);
			if (RES < 0.75f) return 1.f - RES;
			return 1.f - (4.f * RES + 1.f);
		}
	};
}// namespace Formula