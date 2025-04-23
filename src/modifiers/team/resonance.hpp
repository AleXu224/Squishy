#pragma once

#include "formula/operators.hpp"// IWYU pragma: keep
#include "modifiers/enemyFactory.hpp"
#include "modifiers/helpers.hpp"
#include "modifiers/statFactory.hpp"
#include "stats/team.hpp"


namespace Modifiers::Team {
	template<SheetMember<Stats::Sheet<Stats::Value<float, 1>>> stat, SheetMemberIdentifier member>
	struct ResonanceFrm {
		[[nodiscard]] Formula::Compiled::FloatNode compile(const Formula::Context &context) const {
			return stat.resolve(context.team.resonances).compile(context);
		}

		[[nodiscard]] std::string print(const Formula::Context &context, Formula::Step) const {
			return Formula::Percentage("Artifact Substat", eval(context), member.isPercentage());
		}

		[[nodiscard]] constexpr float eval(const Formula::Context &context) const {
			return stat.resolve(context.team.resonances).get(context);
		}
	};
	template<SheetMember<Stats::EnemySheet<Stats::Value<float, 1>>> stat, SheetMemberIdentifier member>
	struct EnemyResonanceFrm {
		[[nodiscard]] Formula::Compiled::FloatNode compile(const Formula::Context &context) const {
			return stat.resolve(context.team.resonancesEnemy).compile(context);
		}

		[[nodiscard]] std::string print(const Formula::Context &context, Formula::Step) const {
			return Formula::Percentage("Artifact Substat", eval(context), member.isPercentage());
		}

		[[nodiscard]] constexpr float eval(const Formula::Context &context) const {
			return stat.resolve(context.team.resonancesEnemy).get(context);
		}
	};

	static constexpr Modifiers::StatFactory<ResonanceFrm, StatPointerFactory<Stats::Sheet<Stats::Value<float, 1>>>{}, StatNameFactory{}> resonances;
	static constexpr EnemyFactory<EnemyResonanceFrm, EnemyPointerFactory<Stats::EnemySheet<Stats::Value<float, 1>>>{}, EnemyNameFactory{}> resonancesEnemy;
}// namespace Modifiers::Team